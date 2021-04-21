/*
Referenèné linky
github.com/rajandeepsingh13/Face-Detection-using-OpenCV-CPP-in-Unity
forum.unity.com/threads/tutorial-using-c-opencv-within-unity.459434/
thomasmountainborn.com/2017/03/05/unity-and-opencv-part-three-passing-detection-data-to-unity/
forum.unity.com/threads/opencvsharp-for-unity.278033/
www.opencv-srf.com/p/introduction.html
*/

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
//umožòuje volanie openCV funkcii
using namespace cv;

//Klasifikátor
CascadeClassifier _frontalFaceCascade;
//Názov okna, ktoré sa pri spustení UNITY3D otvorí
String _windowName = "OPENCV IN UNITY3D";
//Kamera
VideoCapture _capture;
//Škálovanie
int _scale = 1;

/*
Dátová štruktúra, ktorá umožní vykreslenie štvorca okolo tváre
Parametre sú dåžka a šírka tváre
*/
struct Square
{
	Square(int x, int y) : X(x), Y(y) {}
	int X, Y;
};

/*
Naèítanie obrazu z kamery
*/
extern "C" int __declspec(dllexport) __stdcall  Init(int& outCamWidth, int& outCamHeight)
{
	// Naèítanie kaskády s názvom frontalface
	if (!_frontalFaceCascade.load("lbpcascade_frontalface.xml"))
		return -1;

	// Otvorenie streamu
	_capture.open(0);
	if (!_capture.isOpened())
		return -2;

	outCamWidth = _capture.get(CAP_PROP_FRAME_WIDTH);
	outCamHeight = _capture.get(CAP_PROP_FRAME_HEIGHT);

	return 0;
}

/*
Ukonèenie kamery
*/

extern "C" void __declspec(dllexport) __stdcall  Close()
{
	_capture.release();
}

/*
Nastavenie škálovania
*/

extern "C" void __declspec(dllexport) __stdcall SetScale(int scale)
{
	_scale = scale;
}

/*
Detekcia
*/
extern "C" void __declspec(dllexport) __stdcall Detect(Square * outFaces, int maxOutFacesCount, int& outDetectedFacesCount)
{
	Mat frame;
	_capture >> frame;
	if (frame.empty())
		return;

	std::vector<Rect> faces;
	// Konvertuje frame do stupòov šedej.
	Mat grayscaleFrame;
	cvtColor(frame, grayscaleFrame, COLOR_BGR2GRAY);
	Mat resizedGray;
	// Škálovanie smerom dolu pre lepší výkon.
	resize(grayscaleFrame, resizedGray, Size(frame.cols / _scale, frame.rows / _scale));
	equalizeHist(resizedGray, resizedGray);

	// Detekcia.
	_frontalFaceCascade.detectMultiScale(resizedGray, faces);

	// Vykreslenie.
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point vertex1(faces[i].x, faces[i].y);
		Point vertex2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

		rectangle(frame, vertex1, vertex2, Scalar(0, 255, 0), 2, 8, 0);

		outFaces[i] = Square(faces[i].x, faces[i].y);
		outDetectedFacesCount++;

		if (outDetectedFacesCount == maxOutFacesCount)
			break;
	}

	imshow(_windowName, frame);
}

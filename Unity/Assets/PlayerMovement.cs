using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    private float _camDistance;
    //private float speed;
    public Rigidbody rb;
    public float forwardForce = 200f;

    void Start()
    {
        _camDistance = Vector3.Distance(Camera.main.transform.position, transform.position);
    }
    //public float SetSpeed(float speed)
    //{
    //    return speed;
    //}
    //Update
    void FixedUpdate()
    {
  
       rb.AddForce(0,0,forwardForce*Time.deltaTime);
        
        if (OpenCVFaceDetection.NormalizedFacePositions.Count == 0)
            return;

        transform.position = ((new Vector3(OpenCVFaceDetection.NormalizedFacePositions[0].x,transform.position.y, transform.position.z)));

    }
}

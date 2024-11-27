#pragma once

#include <GLFW/glfw3.h>

#include <toolbox/Matrix4x4.h>
#include <toolbox/Vector3.h>

#include "utils/flag.h"

/// <summary>
/// Camera Class
/// </summary>
class UNDEFINED_ENGINE Camera
{
public:
    /// <summary>
    /// Constructor of Camera
    /// </summary>
    /// <param name="width">: Width of the camera</param>
    /// <param name="height">: Height of the camera</param>
    Camera(float width, float height);
    /// <summary>
    /// Destructor of Camera
    /// </summary>
    ~Camera();

    /// <summary>
    /// Set the perspective matrix
    /// </summary>
    /// <param name="perspectiveView">: New perspective matrix</param>
    void SetPerspective(const Matrix4x4& perspectiveView);
    /// <summary>
    /// Set this camera as the pointer to the current camera
    /// </summary>
    void SetCurrentCamera();
    /// <summary>
    /// Set the View matrix
    /// </summary>
    void SetView(const Matrix4x4& viewMatrix);

    /// <summary>
    /// Update the camera
    /// </summary>
    void Update();

    /// <summary>
    /// Get the view matrix
    /// </summary>
    /// <returns>Return the view matrix</returns>
    const Matrix4x4& GetView() const;
    /// <summary>
    /// Get the projection matrix
    /// </summary>
    /// <returns>Return the projection matrix</returns>
    const Matrix4x4& GetProjection() const;
    /// <summary>
    /// Get the VP matrix
    /// </summary>
    /// <returns>Return the VP matrix</returns>
    const Matrix4x4& GetVP() const;

    /// <summary>
    /// Vector3 for the camera eye
    /// </summary>
    Vector3 Eye;
    /// <summary>
    /// Vector3 for the camera LookAt
    /// </summary>
    Vector3 LookAt;
    /// <summary>
    /// Vector3 for the camera Up
    /// </summary>
    Vector3 Up;
  
    /// <summary>
    /// Camera width
    /// </summary>
    float Width;
    /// <summary>
    /// Camera height
    /// </summary>
    float Height;

private:
    /// <summary>
    /// Matrix4x4 for the VP (Perspective * View)
    /// </summary>
    Matrix4x4 mVP;
    /// <summary>
    /// Matrix4x4 for the View
    /// </summary>
    Matrix4x4 mView;
    /// <summary>
    /// Matrix4x4 for the Perspective
    /// </summary>
    Matrix4x4 mPerspective;

    /// <summary>
    /// Speed of the camera
    /// </summary>
    float mCameraSpeed = 0.05f;
    /// <summary>
    /// Sensivity of the mouse when using the camera
    /// </summary>
    float mMouseSensitivity = 0.05f;

    /// <summary>
    /// Eular angle yaw of the camera
    /// </summary>
    float mYaw = -90.f;
    /// <summary>
    /// Eular angle pitch of the camera
    /// </summary>
    float mPitch = 0.f;

    /// <summary>
    /// Last position X of the mouse
    /// </summary>
    float mLastX = 0.f;
    /// <summary>
    /// Last position Y of the mouse
    /// </summary>
    float mLastY = 0.f;
    
    /// <summary>
    /// Boolean to know if the camera is active
    /// </summary>
    bool mIsActive = true;
    /// <summary>
    /// Boolean to know if it is the first time we move the camera since we stop mooving it
    /// </summary>
    bool mIsFirstMouse = true;
    /// <summary>
    /// Boolean to know if we are using the camera for the mouse or if we are using it to navigate through the Editor
    /// </summary>
    bool mIsMouseForCam = false;

public:
    /// <summary>
    /// Callback function for the mouse position to the camera
    /// </summary>
    /// <param name="window">: The pointer to the GLFWwindow used</param>
    /// <param name="xposIn">: Will be fill with mouse posX on screen</param>
    /// <param name="yposIn">: Will be fill with mouse posY on screen</param>
    static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
    /// <summary>
    /// Callback function to modify the speed of the currennt camera using the mouse wheel
    /// </summary>
    /// <param name="window">: The pointer to the GLFWwindow used</param>
    /// <param name="xposIn">: Will be fill with mouse posX on screen</param>
    /// <param name="yposIn">: Will be fill with mouse posY on screen</param>
    static void ChangeSpeedCam(GLFWwindow* window, double xposIn, double yposIn);
    /// <summary>
    /// Manage the input of the current camera
    /// </summary>
    static void ProcessInput();

    /// <summary>
    /// Pointer to the current camera used
    /// </summary>
    static inline Camera* CurrentCamera;
};

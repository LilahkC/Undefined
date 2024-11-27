#include "camera/camera.h"

#include <iostream>
#include <glfw/glfw3.h>
#include <toolbox/calc.h>

#include "service_locator.h"

#include "engine_debug/logger.h"

Camera::Camera(const float width, const float height)
    : Width(width), Height(height)
{
    Eye = Vector3(0, 0.5f, -1);
    LookAt = Vector3(0, 0, -1);
    Up = Vector3(0, 1, 0);

    mPerspective = Matrix4x4::ProjectionMatrix(calc::PI / 2, Width / Height, 0.1f, 100.0f);

    CurrentCamera = this;
}

Camera::~Camera()
{
}

void Camera::SetPerspective(const Matrix4x4& perspectiveMat)
{
    mPerspective = perspectiveMat;
}

const Matrix4x4& Camera::GetView() const
{
    return mView;
}

const Matrix4x4& Camera::GetProjection() const
{
    return mPerspective;
}

const Matrix4x4& Camera::GetVP() const
{
    return mVP;
}

void Camera::SetCurrentCamera()
{
    CurrentCamera = this;
}

void Camera::SetView(const Matrix4x4& viewMatrix)
{
    mView = viewMatrix;
}

void Camera::Update()
{
    mView = Matrix4x4::ViewMatrix(Eye, Eye + LookAt, Up);
    mVP = mPerspective * mView;
}

void Camera::ProcessInput()
{
    if (!CurrentCamera)
        return;

    std::shared_ptr<KeyInput> editorCameraInput = ServiceLocator::Get<InputManager>()->GetKeyInput("editorCameraInput");

    CurrentCamera->mIsMouseForCam = editorCameraInput->GetIsKeyDown(GLFW_MOUSE_BUTTON_RIGHT);

    if (editorCameraInput->GetIsKeyDown(GLFW_KEY_W))
    {
        CurrentCamera->Eye += CurrentCamera->LookAt * CurrentCamera->mCameraSpeed;
    }
    if (editorCameraInput->GetIsKeyDown(GLFW_KEY_S))
    {
        CurrentCamera->Eye -= CurrentCamera->LookAt * CurrentCamera->mCameraSpeed;
    }
    if (editorCameraInput->GetIsKeyDown(GLFW_KEY_A))
    {
        CurrentCamera->Eye -= Vector3::Cross(CurrentCamera->LookAt, CurrentCamera->Up).Normalized() * CurrentCamera->mCameraSpeed;
    }
    if (editorCameraInput->GetIsKeyDown(GLFW_KEY_D))
    {
        CurrentCamera->Eye += Vector3::Cross(CurrentCamera->LookAt, CurrentCamera->Up).Normalized() * CurrentCamera->mCameraSpeed;
    }
    if (editorCameraInput->GetIsKeyDown(GLFW_KEY_SPACE))
    {
        CurrentCamera->Eye.y += CurrentCamera->mCameraSpeed;
    }
    if (editorCameraInput->GetIsKeyDown(GLFW_KEY_LEFT_SHIFT))
    {
        CurrentCamera->Eye.y -= CurrentCamera->mCameraSpeed;
    }
}

void Camera::MouseCallback(GLFWwindow* const window, const double xposIn, const double yposIn)
{
    if (!CurrentCamera)
        return;

    InputManager* inputManager = ServiceLocator::Get<InputManager>();

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (CurrentCamera->mIsMouseForCam)
    {
        inputManager->SetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        inputManager->SetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        CurrentCamera->mIsFirstMouse = true;
        return;
    }

    if (CurrentCamera->mIsFirstMouse)
    {
        CurrentCamera->mLastX = xpos;
        CurrentCamera->mLastY = ypos;
        CurrentCamera->mIsFirstMouse = false;
    }

    float xoffset = xpos - CurrentCamera->mLastX;
    float yoffset = CurrentCamera->mLastY - ypos;
    CurrentCamera->mLastX = xpos;
    CurrentCamera->mLastY = ypos;

    xoffset *= CurrentCamera->mMouseSensitivity;
    yoffset *= CurrentCamera->mMouseSensitivity;

    CurrentCamera->mYaw += xoffset;
    CurrentCamera->mPitch += yoffset;

    if (CurrentCamera->mPitch > 89.0f)
    {
        CurrentCamera->mPitch = 89.0f;
    }
    if (CurrentCamera->mPitch < -89.0f)
    {
        CurrentCamera->mPitch = -89.0f;
    }

    Vector3 direction;
    direction.x = cosf((CurrentCamera->mYaw * (calc::PI / 180.f))) * cosf((CurrentCamera->mPitch * (calc::PI / 180.f)));
    direction.y = sinf((CurrentCamera->mPitch * (calc::PI / 180.f)));
    direction.z = sinf((CurrentCamera->mYaw * (calc::PI / 180.f))) * cosf((CurrentCamera->mPitch * (calc::PI / 180.f)));

    CurrentCamera->LookAt = direction.Normalized();

    
}

void Camera::ChangeSpeedCam(GLFWwindow* , double , double yposIn)
{
    if (!CurrentCamera)
        return;

    CurrentCamera->mCameraSpeed += calc::Sign((float)yposIn) * 0.0025f;

    // verify if camera speed is not negative so that we don't go opposite of where we want
    if (CurrentCamera->mCameraSpeed < 0.f)
    {
        CurrentCamera->mCameraSpeed = 0.0001f;
    }

    Logger::Info("Camera speed : {}", CurrentCamera->mCameraSpeed);
}

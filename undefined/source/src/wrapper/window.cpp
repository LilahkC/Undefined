#include "wrapper/window.h"

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#include <iostream>
#include <toolbox/calc.h>
#include <stb_image/stb_image.h>

#include "engine_debug/logger.h"
#include "service_locator.h"

bool fullscreen;

#ifdef DEBUG
fullscreen = true;
#endif // DEBUG
#ifdef RELEASE
fullscreen = false;
#endif // RELEASE

Window::Window()
{
    mWindow = NULL;
}

Window::~Window()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Window::Init()
{
    SetupWindowAPI();

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // if we are in debug create a widow of size 1200*800 but in release create a window if the screen size wich is in fullscreen
    CreateWindow(fullscreen == true ? mode->width : 1200, fullscreen == true ? mode->height : 800, fullscreen == true ? monitor : nullptr);

    SetupWindow();
}

void Window::SetupWindowAPI()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    glfwSetErrorCallback(
        [](int error, const char* description)
        {
            Logger::FatalError("GLFW error {} : {}", error, description);
        }
    );

    if (!glfwInit())
    {
        Logger::FatalError("GLFW could not be initialized");
    }
}

void Window::CreateWindow(int width, int height, GLFWmonitor* monitor)
{
    mWindow = glfwCreateWindow(width, height, "Undefined Engine", monitor, nullptr);

    if (!mWindow)
    {
        Logger::FatalError("Window not initialized");
    }

    //Add an icon to our window
    GLFWimage images;
    images.pixels = stbi_load("../Undefined/resource_manager/assets/undefined_logo.png", &images.width, &images.height, 0, 4); //rgba channels

    glfwSetWindowIcon(mWindow, 1, &images);

    stbi_image_free(images.pixels);

    Width = width;
    Height = height;
}

void Window::SetupWindow()
{
    glfwMakeContextCurrent(mWindow);

    glfwSwapInterval(1); // Enable vsync
}

void Window::GetFramebufferSize(int& display_width, int& display_height)
{
    glfwGetFramebufferSize(mWindow, &display_width, &display_height);
}

GLFWwindow* Window::GetWindowPointer()
{
    return mWindow;
}

bool Window::IsWindowOpen()
{
    return !glfwWindowShouldClose(mWindow) && glfwGetKey(mWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

void Window::WindowSizeCallback(GLFWwindow* , int width, int height)
{
    Window* w = ServiceLocator::Get<Window>();

    w->Width = width;
    w->Height = height;

    if (w->Height < 0 || w->Width < 0)
    {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::SetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun callback)
{
    glfwSetWindowSizeCallback(window, callback);
}

void Window::ScrollWheelCallback(GLFWwindow* window, GLFWscrollfun callback)
{
    glfwSetScrollCallback(window, callback);
}

void Window::Callbacks()
{
    Window* w = ServiceLocator::Get<Window>();

    Window::SetWindowSizeCallback(w->GetWindowPointer(), Window::WindowSizeCallback);
    Window::ScrollWheelCallback(w->GetWindowPointer(), Camera::ChangeSpeedCam);
}
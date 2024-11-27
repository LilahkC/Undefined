#pragma once

#include "wrapper/service_type.h"

#include "utils/flag.h"

#include "camera/camera.h"

struct GLFWwindow;

/// <summary>
/// Class for the Window
/// </summary>
class UNDEFINED_ENGINE Window : public ServiceType
{
public:
    /// <summary>
    /// Constructor of Window
    /// </summary>
    Window();
    /// <summary>
    /// Destructor of Window
    /// </summary>
    ~Window();

    /// <summary>
    /// Init the window
    /// </summary>
    void Init();

    /// <summary>
    /// setup the Window API
    /// </summary>
    void SetupWindowAPI();

    /// <summary>
    /// Create a new Window
    /// </summary>
    /// <param name="width">: Width of the window</param>
    /// <param name="height">: Height of the window</param>
    /// <param name="monitor">: monitor used by the Window where you want to fullscreen (by default : nullptr, so no fullscreen)</param>
    void CreateWindow(int width, int height, GLFWmonitor* monitor = nullptr);

    /// <summary>
    /// Setup the Window context qnd interval
    /// </summary>
    void SetupWindow();

    /// <summary>
    /// Get the Window framebuffer size
    /// </summary>
    /// <param name="displayWidth">: Value where the width will be written</param>
    /// <param name="displayHeight">: Value where the height will be written</param>
    void GetFramebufferSize(int &displayWidth, int &displayHeight);

    /// <summary>
    /// Get the GLFWwindow pointer
    /// </summary>
    /// <returns>Return the GLFWwindow pointer</returns>
    GLFWwindow* GetWindowPointer();

    /// <summary>
    /// Checks if the Window is open
    /// </summary>
    /// <returns>Return either true if it is open or false</returns>
    bool IsWindowOpen();

    /// <summary>
    /// Swap the buffers
    /// </summary>
    void SwapBuffers();

    /// <summary>
    /// Width of the Window
    /// </summary>
    int Width = 0;
    /// <summary>
    /// Height of the Window
    /// </summary>
    int Height = 0;

private:
    /// <summary>
    /// Pointer to the GLFWwindow
    /// </summary>
    GLFWwindow* mWindow;

public:
    /// <summary>
    /// Function for the Window size
    /// </summary>
    /// <param name="width">: New window width</param>
    /// <param name="height">: New window height</param>
    static void WindowSizeCallback(GLFWwindow* , int width, int height);
    /// <summary>
    /// Callback to set the Window size
    /// </summary>
    /// <param name="window">: Pointer to the GLFWwindow</param>
    /// <param name="callback">: Callback function</param>
    static void SetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun callback);
    /// <summary>
    /// Callback to set the scroll wheel value
    /// </summary>
    /// <param name="window">: Pointer to the GLFWwindow</param>
    /// <param name="callback">: Callback function</param>
    static void ScrollWheelCallback(GLFWwindow* window, GLFWscrollfun callback);

    /// <summary>
    /// All our Window callabacks are here
    /// </summary>
    static void Callbacks();
};
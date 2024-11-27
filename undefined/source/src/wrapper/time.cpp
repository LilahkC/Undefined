#include "wrapper/time.h"

#include <glfw/glfw3.h>
#include <algorithm>

double Time::GetTimeSinceLaunch()
{
    return glfwGetTime();
}


void Time::SetTimeVariables()
{
    float delta = float(GetTimeSinceLaunch() - mLastTimeSinceLauch);

    mDeltaTime = std::min(delta, MaxDeltaTime) * TimeScale;

    FixedStep += (delta / FixedDeltaTime) * TimeScale;

    mLastTimeSinceLauch = glfwGetTime();
}
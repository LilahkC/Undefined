#pragma once
#include "utils/flag.h"

class Time
{
    
    STATIC_CLASS(Time)

private:

    UNDEFINED_ENGINE static inline float mDeltaTime; // Read Only
    
public:

    UNDEFINED_ENGINE static double GetTimeSinceLaunch();


    UNDEFINED_ENGINE static inline float TimeScale = 1.f;

    UNDEFINED_ENGINE static inline const float& DeltaTime = Time::mDeltaTime; // Read Only
    //UNDEFINED_ENGINE static float GetDeltaTime;

    UNDEFINED_ENGINE static inline float FixedDeltaTime = 1.2f / 60.f; // Read Write

    static inline float FixedStep = 0; // Read Write but not by user

    UNDEFINED_ENGINE static inline float MaxDeltaTime = 20.f / 60.f; // Read Write

private:

    friend class Application;
    static void SetTimeVariables();

    static inline double mLastTimeSinceLauch;

};
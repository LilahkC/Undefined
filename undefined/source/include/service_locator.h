#pragma once

#include <unordered_map>

#include "wrapper/window.h"
#include "wrapper/renderer.h"
#include "wrapper/input_manager.h"

#include "utils/flag.h"

/// <summary>
/// Service Locator Class to get our Services (e.g : Renderer, InputManager, ...)
/// </summary>
class ServiceLocator
{
    STATIC_CLASS(ServiceLocator)

public:
    /// <summary>
    /// Setup the Service Locator
    /// </summary>
    UNDEFINED_ENGINE static void Setup();

    /// <summary>
    /// Setup the Callbacks from the Services
    /// </summary>
    /// <returns></returns>
    UNDEFINED_ENGINE static void SetupCallbacks();

    /// <summary>
    /// Provide a Service to the ServiceLocator
    /// </summary>
    /// <typeparam name="ServiceType">: Type of the Service (e.g : Renderer, InputManager, ...)</typeparam>
    /// <param name="p_service"></param>
    template<typename ServiceType>
    static void Provide(ServiceType* const p_service)
    {
        mServices[typeid(ServiceType).hash_code()] = p_service;
    }

    /// <summary>
    /// Get a Service
    /// </summary>
    /// <typeparam name="ServiceType">: Type of the Service (e.g : Renderer, InputManager, ...)</typeparam>
    /// <returns>Return a pointer to the Service</returns>
    template<typename ServiceType>
    static ServiceType* Get()
    {
        return reinterpret_cast<ServiceType*>(mServices[typeid(ServiceType).hash_code()]);
    }

    /// <summary>
    /// Delete all the Services in the ServiceLocator
    /// </summary>
    UNDEFINED_ENGINE static void CleanServiceLocator();

private:
    /// <summary>
    /// Map of the services with the hash_code as the key
    /// </summary>
    UNDEFINED_ENGINE static inline std::unordered_map<size_t, ServiceType*> mServices;
};


#pragma once

#include <glfw/glfw3.h>
#include <map>
#include <vector>

#include "utils/flag.h"

/// <summary>
/// A Key Input that manage a set of keys for a purpose (e.g : a KeyInput per player, a KeyInput for the menu, ...)
/// </summary>
class KeyInput
{
public:
    /// <summary>
    /// Constructor of KeyInput
    /// </summary>
    /// <param name="keysToMonitor">: std::vector of the key you want to monitor in this KeyInput</param>
    UNDEFINED_ENGINE KeyInput(const std::vector<int>& keysToMonitor);
    /// <summary>
    /// Destructor of KeyInput
    /// </summary>
    UNDEFINED_ENGINE ~KeyInput();

    /// <summary>
    /// Check if a key is down
    /// </summary>
    /// <param name="key">: Key you want to check</param>
    /// <returns>Return either true if it is down or false</returns>
    UNDEFINED_ENGINE const bool GetIsKeyDown(int key);
    /// <summary>
    /// Get if the KeyInput is enabled
    /// </summary>
    /// <returns>Return either true if it is enable or false</returns>
    UNDEFINED_ENGINE bool GetIsEnabled();
    /// <summary>
    /// Set if the KeyInput is enabled or not
    /// </summary>
    /// <param name="value">: Value for if the KeyInput is enable</param>
    UNDEFINED_ENGINE void SetIsEnabled(bool value);

    /// <summary>
    /// Set a key as down
    /// </summary>
    /// <param name="key">: Key you want to set</param>
    /// <param name="isDown">: Value you want to set</param>
    UNDEFINED_ENGINE void SetIsKeyDown(int key, bool isDown);

private:
    /// <summary>
    /// std::map of the key in the KeyInput and their states
    /// </summary>
    std::map<int, bool> mKeysMap;
    /// <summary>
    /// Boolean value to know if the KeyInput is enable
    /// </summary>
    bool mIsEnabled;

private:
    /// <summary>
    /// InputManager is a friend class from KeyInput
    /// </summary>
    friend class InputManager;
};

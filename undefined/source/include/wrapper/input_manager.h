#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "wrapper/service_type.h"
#include "wrapper/key_input.h"
#include "utils/flag.h"

/// <summary>
/// Class for the InputManager which inherit from ServiceType in order to be store in the ServiceLocator
/// </summary>
class InputManager : public ServiceType
{
public:
	/// <summary>
	/// constructor of InputManager
	/// </summary>
	UNDEFINED_ENGINE InputManager();

	/// <summary>
	/// Create a KeyInput
	/// </summary>
	/// <param name="keyInputName">: Name of the KeyInput</param>
	/// <param name="keysToMonitor">: Keys to monitor in the KeyInput</param>
	UNDEFINED_ENGINE void CreateKeyInput(const std::string& keyInputName, const std::vector<int>& keysToMonitor);

	/// <summary>
	/// Delete a KeyInput by his name
	/// </summary>
	/// <param name="keyInputName">: Name of the KeyInput</param>
	UNDEFINED_ENGINE void DeleteKeyInput(const std::string& keyInputName);

	/// <summary>
	/// Set the window input mode
	/// </summary>
	/// <param name="window">: Pointer to the window</param>
	/// <param name="mode">: Input mode to modify</param>
	/// <param name="value">: Value of the input mode</param>
	UNDEFINED_ENGINE void SetInputMode(GLFWwindow* window, int mode, int value);

	/// <summary>
	/// Get a KeyInput
	/// </summary>
	/// <param name="keyInputName">: Name of the KeyInput</param>
	/// <returns>Return a pointer to the KeyInput</returns>
	UNDEFINED_ENGINE std::shared_ptr<KeyInput> GetKeyInput(const std::string& keyInputName);

	/// <summary>
	/// An std::unordered_map to store all our KeyInputsMap
	/// </summary>
	std::unordered_map<std::string, std::shared_ptr<KeyInput>> KeyInputsMap;

public:
	/// <summary>
	/// All our Callbacks are in that function
	/// </summary>
	UNDEFINED_ENGINE static void Callbacks();

private:
	/// <summary>
	/// Callback for the keyboard input
	/// </summary>
	/// <param name="window">: Pointer to the window</param>
	/// <param name="key">: Key get during the callback</param>
	/// <param name="action">: Action of the key</param>
	UNDEFINED_ENGINE static void Callback(GLFWwindow* window, int key, int, int action, int);

	/// <summary>
	/// Callback for the mouse button
	/// </summary>
	/// <param name="button">: Button get during the callback</param>
	/// <param name="action">: Action of the key</param>
	UNDEFINED_ENGINE static void MouseButtonCallback(GLFWwindow*, int button, int action, int);

	/// <summary>
	/// All the KeyInput instances
	/// </summary>
	UNDEFINED_ENGINE static inline std::vector<std::shared_ptr<KeyInput>> mInstances;
};
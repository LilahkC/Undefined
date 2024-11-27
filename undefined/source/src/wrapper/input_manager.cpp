#include "wrapper/input_manager.h"

#include <iostream>
#include <cassert>

// not sure about that
#include "application.h"

#include "service_locator.h"

#include "engine_debug/logger.h"

InputManager::InputManager()
{
}

void InputManager::CreateKeyInput(const std::string& keyInputName, const std::vector<int>& keysToMonitor)
{
	std::shared_ptr<KeyInput> keyInput = std::make_shared<KeyInput>(keysToMonitor);

	auto&& p = KeyInputsMap.try_emplace(keyInputName, keyInput);

	if (!p.second)
	{
		p.first->second->~KeyInput();
		keyInput = std::make_shared<KeyInput>(keysToMonitor);
	}

	KeyInputsMap.emplace(keyInputName, keyInput);
	mInstances.push_back(keyInput);
}

void InputManager::DeleteKeyInput(const std::string& keyInputName)
{
	mInstances.erase(std::remove(mInstances.begin(), mInstances.end(), GetKeyInput(keyInputName)), mInstances.end());
}

void InputManager::SetInputMode(GLFWwindow* window, int mode, int value)
{
	glfwSetInputMode(window, mode, value);
}

std::shared_ptr<KeyInput> InputManager::GetKeyInput(const std::string& keyInputName)
{
	auto&& p = KeyInputsMap.find(keyInputName);

	if (p == KeyInputsMap.end())
	{
		Logger::Error("Key Input name incorrect : {} ", keyInputName);
		assert(false);
		return nullptr;
	}

	return p->second;
}

void InputManager::Callbacks()
{
	GLFWwindow* mWindowManager = ServiceLocator::Get<Window>()->GetWindowPointer();

	glfwSetKeyCallback(mWindowManager, InputManager::Callback);
	glfwSetMouseButtonCallback(mWindowManager, InputManager::MouseButtonCallback);
	glfwSetCursorPosCallback(mWindowManager, Camera::MouseCallback);
}

void InputManager::Callback(GLFWwindow*, int key, int, int action, int)
{
	bool isInInstances = false;

	for (std::shared_ptr<KeyInput> keyInput : mInstances)
	{
		std::map<int, bool>::iterator it = keyInput->mKeysMap.find(key);

		if (it != keyInput->mKeysMap.end())
		{
			keyInput->SetIsKeyDown(key, action != GLFW_RELEASE);
			isInInstances = true;
		}
	}
	
	if (Application::IsInGame == true)
	{
		if (isInInstances == false)
		{
			if (key >= 65 && key <= 90)
			{
				Logger::Error("Key {} is not in the KeyInput", static_cast<char>(key));
			}
			else
			{
				Logger::Error("A key that is not a letter is not in a KeyInput");
			}
		}
	}
}

void InputManager::MouseButtonCallback(GLFWwindow*, int button, int action, int)
{
	bool isInInstances = false;

	for (std::shared_ptr<KeyInput> keyInput : mInstances)
	{
		std::map<int, bool>::iterator it = keyInput->mKeysMap.find(button);

		if (it != keyInput->mKeysMap.end())
		{
			keyInput->SetIsKeyDown(button, action != GLFW_RELEASE);
			isInInstances = true;
		}
	}

	if (Application::IsInGame == true)
	{
		if (isInInstances == false)
		{
			Logger::Error("Mouse button {} is not in the KeyInput", button + 1);
		}
	}
}
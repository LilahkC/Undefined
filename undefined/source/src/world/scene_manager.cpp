#include "world/scene_manager.h"

#include <json/json.h>
#include <iostream>
#include <fstream>

#include "wrapper/time.h"
#include "wrapper/physics_system.h"
#include "reflection/utils_reflection.h"

#include "world/point_light.h"

void SceneManager::Init()
{
	if (!Object::mRoot)
	{
		Object::mRoot = new Object("Root");
	}
	if (!ActualScene)
	{
		ActualScene = CreateScene("Default Scene");
	}
}

void SceneManager::Delete()
{
	delete Object::mRoot;
	for (Scene* scene : Scenes)
	{
		delete scene;
	}
}

Scene* SceneManager::CreateScene(const std::string& mName)
{
	Object::mRoot->DetachChildren();
	delete ActualScene;

	ActualScene = new Scene(mName);
	return ActualScene;
}

void SceneManager::SetPlay(bool play)
{
	IsScenePlaying = play;
}

void SceneManager::SetPause(bool pause)
{
	IsScenePaused = pause;
}

void SceneManager::Start()
{
	if (!ActualScene)
	{
		Logger::Error("No scene loaded");
		return;
	}
	// save ActualScene into temp scene (to restart)
	SceneManager::SaveTempScene();

	ActualScene->Start();
}

void SceneManager::GlobalUpdate()
{
	if (!ActualScene)
	{
		Logger::Error("No scene loaded");
		return;
	}

	if (!IsScenePlaying)
	{
		while (Time::FixedStep >= 1)
		{
			Time::FixedStep--;
		}

		for (Object* obj : ActualScene->Objects)
		{
			obj->mTransform.UpdateTransform();
		}
		return;
	}

	if (IsScenePaused /*&& !nextFrameButton*/)
	{
		while (Time::FixedStep >= 1)
		{
			Time::FixedStep--;
		}

		for (Object* obj : ActualScene->Objects)
		{
			obj->mTransform.UpdateTransform();
		}
		return;
	}

	while (Time::FixedStep >= 1)
	{
		ActualScene->FixedUpdate();
		ActualScene->PreFixedUpdate();
		PhysicsSystem::Update();
		ActualScene->PostFixedUpdate();
		Time::FixedStep--;
	}

	ActualScene->Update();
	ActualScene->LateUpdate();

	for (Object* obj : ActualScene->Objects)
	{
		obj->mTransform.UpdateTransform();
	}
}

void SceneManager::Draw()
{
	if (!ActualScene)
	{
		Logger::Error("No scene loaded");
		return;
	}

	ActualScene->Draw();
	ActualScene->PostDraw();
}

void SceneManager::SaveTempScene()
{
	if (!ActualScene)
	{
		return;
	}

	Json::Value root;

	for (Object* obj : ActualScene->Objects)
	{
		root[std::to_string(obj->mUUID)] = Reflection::WriteObj(obj);
	}

	std::ofstream file("temp.scene");
	file << root.toStyledString();

	file.close();
}

void SceneManager::SaveCurrentScene()
{
	if (!ActualScene)
	{
		return;
	}

	Json::Value root;

	for (Object* obj : ActualScene->Objects)
	{
		root[std::to_string(obj->mUUID)] = Reflection::WriteObj(obj);
	}

	//std::ofstream file("assets/scenes/test.scene");
	if (!ActualScene->Path.string().ends_with(".scene"))
	{
		ActualScene->Path = "assets/scenes/" + ActualScene->Name + ".scene";
	}
	std::ofstream file(ActualScene->Path);
	file << root.toStyledString();

	file.close();
}

bool SceneManager::LoadScene(const std::filesystem::path& path)
{
	if (!path.string().ends_with(".scene"))
	{
		Logger::Error("Can't load scene : {}", path.generic_string());
		return false;
	}

	Json::Value root;
	std::ifstream file(path);

	if (!file.is_open())
	{
		Logger::Error("Can't load scene : {}", path.generic_string());
		return false;
	}

	Object::mRoot->DetachChildren();
	delete ActualScene;

	std::string sceneName = path.string().erase(0, 14);
	size_t offset = sceneName.find(".scene");
	ActualScene = new Scene(sceneName.erase(offset, 6));
	ActualScene->Path = path;

	file >> root;

	std::vector<std::string> names = root.getMemberNames();
	for (size_t i = 0; i < root.size(); i++)
	{
		Object* obj = Reflection::ReadObj<Object>(root.get(names[i], Json::Value()));
		obj->mTransform.mHasChanged = true;
		ActualScene->Objects.push_back(obj);
	}

	for (Object* obj : ActualScene->Objects)
	{
		obj->ResetPointerLink();
	}

	for (Object* obj : ActualScene->Objects)
	{
		if (!obj->mParent)
		{
			obj->SetParent(nullptr);
		}
	}

	file.close();
	return true;
}

void SceneManager::Reload()
{
	std::filesystem::path path = ActualScene->Path;
	std::string name = ActualScene->Name;

	SceneManager::LoadScene("temp.scene");

	ActualScene->Path = path;
	ActualScene->Name = name;
}

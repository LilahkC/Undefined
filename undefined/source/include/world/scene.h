#pragma once

#include "world/object.h"
#include "utils/flag.h"

class Scene
{
public:
	UNDEFINED_ENGINE Scene();
	UNDEFINED_ENGINE Scene(const std::string& mName);
	UNDEFINED_ENGINE ~Scene();

	UNDEFINED_ENGINE void Start();
	UNDEFINED_ENGINE void PreFixedUpdate();
	UNDEFINED_ENGINE void FixedUpdate();
	UNDEFINED_ENGINE void PostFixedUpdate();
	UNDEFINED_ENGINE void Update();
	UNDEFINED_ENGINE void LateUpdate();
	UNDEFINED_ENGINE void Draw();
	UNDEFINED_ENGINE void PostDraw();

	UNDEFINED_ENGINE Object* AddObject(const std::string& mName = "Default");
	UNDEFINED_ENGINE Object* AddObject(Object* parent, const std::string& mName = "Default");
	UNDEFINED_ENGINE Object* AddObject(Vector3 position, Vector3 rotation, const std::string& mName = "Default");
	UNDEFINED_ENGINE Object* AddObject(Vector3 position, Vector3 rotation, Object* parent, bool world = true , const std::string& mName = "Default");
	UNDEFINED_ENGINE Object* AddObject(const Object& original, const std::string& mName = "Default");
	UNDEFINED_ENGINE Object* AddObject(const Object& original, Object* parent, const std::string& mName = "Default");
	UNDEFINED_ENGINE Object* AddObject(const Object& original, Vector3 position, Vector3 rotation, const std::string& mName = "Default");
	UNDEFINED_ENGINE Object* AddObject(const Object& original, Vector3 position, Vector3 rotation, Object* parent, bool world = true, const std::string& mName = "Default");

	UNDEFINED_ENGINE void RemoveObject(Object* object);

	std::string Name = "Default";

	std::filesystem::path Path;

	std::vector<Object*> Objects;
};
#include "world/scene.h"

#include "world/skybox.h"

Scene::Scene()
{
}

Scene::Scene(const std::string& mName)
	: Name(mName)
{
}

Scene::~Scene()
{
	for (Object* object : Objects)
	{
		delete object;
	}
}

void Scene::Start()
{
	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}

			comp->Start();
		}
	}
}

UNDEFINED_ENGINE void Scene::PreFixedUpdate()
{
	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}

			comp->PreFixedUpdate();
		}
	}
}

void Scene::FixedUpdate()
{
	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}

			
			comp->FixedUpdate();
		}
	}
}

UNDEFINED_ENGINE void Scene::PostFixedUpdate()
{
	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}

			comp->PostFixedUpdate();
		}
	}
}

void Scene::Update()
{
	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}

			comp->Update();
		}
	}
}

void Scene::LateUpdate()
{
	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}

			comp->LateUpdate();
		}
	}
}

void Scene::Draw()
{
	Renderer* rend = ServiceLocator::Get<Renderer>();
	std::shared_ptr<Shader> shader = ResourceManager::Get<Shader>("base_shader");

	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}
			rend->UseShader(shader->ID);
			rend->SetUniform(shader->ID, "EntityID", (int)i);
			rend->UnUseShader();
			comp->Draw();
		}
	}

	Skybox::Draw();
}

UNDEFINED_ENGINE void Scene::PostDraw()
{
	for (size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsEnable())
		{
			continue;
		}

		for (Component* comp : Objects[i]->Components)
		{
			if (!comp->IsEnable())
			{
				continue;
			}

			comp->PostDraw();
		}
	}
}

Object* Scene::AddObject(const std::string& mName)
{
	Object* obj = new Object(mName);
	Objects.push_back(obj);
	obj->SetParent(nullptr);

	return obj;
}

Object* Scene::AddObject(Object* parent, const std::string& mName)
{
	Object* obj = new Object(mName);
	Objects.push_back(obj);
	obj->SetParent(parent);

	return obj;
}

Object* Scene::AddObject(Vector3 position, Vector3 rotation, const std::string& mName)
{
	Object* obj = new Object(mName);
	Objects.push_back(obj);
	obj->SetParent(nullptr);
	obj->GameTransform->Position = position;
	obj->GameTransform->Rotation = rotation;

	return obj;
}

Object* Scene::AddObject(Vector3 position, Vector3 rotation, Object* parent, bool world, const std::string& mName)
{
	Object* obj = new Object(mName);
	Objects.push_back(obj);
	obj->SetParent(parent);
	if (world)
	{
		obj->GameTransform->Position = position;
		obj->GameTransform->Rotation = rotation;
	}
	else
	{
		obj->GameTransform->LocalPosition = position;
		obj->GameTransform->LocalRotation = rotation;
	}

	return obj;
}

UNDEFINED_ENGINE void Scene::RemoveObject(Object* object)
{

	if (!object)
	{
		Logger::Error("RemoveObject(Object* object) object is nullptr");
		return;
	}

	int index = 0;
	for (Object* findObj : Objects)
	{
		if (findObj == object)
		{
			Object::mRoot->DetachChild(object);
			Objects.erase(Objects.begin() + index);
			Logger::Info("Object \"{}\" removed", object->Name);
			delete object;
			Objects.shrink_to_fit();
			return;
		}
		index++;
	}
	Logger::Warning("Object to remove not found in scene \"{}\"", Name);
}


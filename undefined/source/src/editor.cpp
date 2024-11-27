#include "editor.h"

Editor::Editor()
{
}

void Editor::Init()
{
	ResourceManager::Load("../undefined/resource_manager/", true);
	ResourceManager::Load("assets/", true);

	Interface::Init();
	Inspector::Init();
	ResourceManager::Get<Model>("assets/viking_room.obj")->SetTexture(0, ResourceManager::Get<Texture>("assets/viking_room.png"));
}

void Editor::Update()
{
	Interface::Update();
}

void Editor::Terminate()
{
	delete Camera::CurrentCamera;
	ResourceManager::UnloadAll();
	Interface::Delete();
}

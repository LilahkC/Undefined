#include "interface/inspector.h"

#include <imgui/imgui.h>

#include "reflection/attributes.h"
#include "reflection/utils_reflection.h"

#include "world/scene_manager.h"
#include "reflection/runtime_classes.h"
#include "world/component.h"

void Inspector::Init()
{
	mRenderer = ServiceLocator::Get<Renderer>();
}

void Inspector::ShowWindow()
{
	ImGui::Begin("Inspector");
	
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
	{
		ServiceLocator::Get<InputManager>()->GetKeyInput("editorCameraInput")->SetIsEnabled(false);
	}

	if (mRenderer->ObjectIndex >= 0)
	{
		Object* obj = SceneManager::ActualScene->Objects[mRenderer->ObjectIndex];
		Reflection::ReflectionObj<Object>(obj);

		if (ImGui::Button("Add Component"))
		{
            ImGui::OpenPopup("component_popup");
		}

		ImGui::SameLine();
        
		if (ImGui::BeginPopup("component_popup"))
        {
			for (int i = 0; i < RuntimeClasses::names.size(); i++)
			{
				if (ImGui::Selectable(RuntimeClasses::names[i].c_str()))
				{
					obj->AddComponent(static_cast<Component*>(RuntimeClasses::CreateClass(RuntimeClasses::names[i])));
				}
			}
            ImGui::EndPopup();
        }
	}

	ImGui::End();
}

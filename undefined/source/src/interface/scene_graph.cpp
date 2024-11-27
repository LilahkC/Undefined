#include "interface/scene_graph.h"

#include <imgui/imgui.h>
#include <toolbox/calc.h>
#include <set>
#include <algorithm>

#include "world/scene_manager.h"

#include "service_locator.h"

#include "resources/model_renderer.h"
#include "resources/resource_manager.h"

#include "world/box_collider.h"
#include "world/capsule_collider.h"

#include "wrapper/time.h"

static inline std::set<ImGuiID> openNextFrame;

void SceneGraph::DisplayWindow()
{
    if (!SceneManager::ActualScene)
    {
        return;
    }

    ImGui::Begin("Hierarchy");

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
    {
        ServiceLocator::Get<InputManager>()->GetKeyInput("editorCameraInput")->SetIsEnabled(false);
    }

    if (ImGui::BeginPopup("LoadString"))
    {
        char* newName = (char*)loadString.c_str();
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;

        ImGui::SetKeyboardFocusHere();
        ImGui::PushItemWidth(10 + ImGui::CalcTextSize(newName).x);
        if (ImGui::InputText("##load", (char*)newName, 256, flags))
        {
            std::string path = "assets/scenes/";
            path += newName;
            if (!path.ends_with(".scene"))
            {
                path.append(".scene");
            }
            ServiceLocator::Get<Renderer>()->ObjectIndex = -1;
            SceneManager::LoadScene(path);
            path = "";
            loadString = "";
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("CreateString"))
    {
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
        char* newName = (char*)loadString.c_str();

        ImGui::SetKeyboardFocusHere();
        ImGui::PushItemWidth(10 + ImGui::CalcTextSize(newName).x);
        if (ImGui::InputText("##load", (char*)newName, 256, flags))
        {
            loadString = newName;
            ServiceLocator::Get<Renderer>()->ObjectIndex = -1;
            SceneManager::CreateScene(loadString);
            loadString = "";
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("Create"))
    {
        ImGui::OpenPopup("CreateString");
    }
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        SceneManager::SaveCurrentScene();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        ImGui::OpenPopup("LoadString");
    }

    if (ImGui::CollapsingHeader(SceneManager::ActualScene->Name.c_str(),
        ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_Leaf))

    DisplayActualScene();

    ImGui::End();
}

void SceneGraph::Delete()
{
    delete mRenamingObject;
    delete mSelectedObject;
}

void SceneGraph::DisplayActualScene()
{
    RightClickObject(Object::mRoot);
    unsigned int labelID = 0;
    for (Object* object : Object::mRoot->GetChildren())
    {
        DisplayObject(object, labelID);
        labelID++;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::Dummy(ImGui::GetContentRegionAvail());
    ImGui::PopStyleVar();
    BeginDropOnObject(Object::mRoot);
}

void SceneGraph::DisplayObject(Object* object, unsigned int labelID)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow;

    if (object->GetChildren().size() < 1)
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (mSelectedObject == object)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::Dummy(ImVec2(ImGui::GetItemRectMax().x, 6.f));
    ImGui::PopStyleVar();

    BeginDropOnObject(object, true);
    if (openNextFrame.contains(ImGui::GetID(object->Name.c_str())))
    {
        openNextFrame.erase(ImGui::GetID(object->Name.c_str()));
        ImGui::SetNextItemOpen(true);
    }

    std::string displayName = object->Name.c_str();
    if (object == mRenamingObject)
    {
        displayName = "##Renaming";
    }
    else if (object->Name == "")
    {
        displayName = "##Empty";
    }
    displayName += "##" + std::to_string(labelID);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    if (ImGui::TreeNodeEx(displayName.c_str(), flags))
    {
        ImGui::PopStyleVar();
        NodeInteraction(object);

        //For every child we call the function to display their children
        for (Object* child : object->GetChildren())
        {
            labelID++;
            DisplayObject(child, labelID);
        }
        ImGui::TreePop();
    }
    else
    {
        ImGui::PopStyleVar();
        NodeInteraction(object);
    }

}

void SceneGraph::NodeInteraction(Object* object)
{
    ClickSelectObject(object);
    if (RightClickObject(object))
    {
        openNextFrame.insert(ImGui::GetItemID());
    }
    BeginDragObject(object);
    if (BeginDropOnObject(object))
    {
        openNextFrame.insert(ImGui::GetItemID());
    }
    RenameObject(object);
}

void SceneGraph::ClickSelectObject(Object* object)
{

    if (!ImGui::IsItemToggledOpen() && ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        mSelectedObject = object;
        for (size_t i = 0; i < SceneManager::ActualScene->Objects.size(); i++) // temp TODO: find
        {
            if (object == SceneManager::ActualScene->Objects[i])
            {
                ServiceLocator::Get<Renderer>()->ObjectIndex = (int)i;
            }
        }
    }
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
    {
        // Time for the travel time of the camera when double clicking to an object 
        mCamTravelTime = 0.4f;
        mLerpCam = true;
        mBaseCamPos = Camera::CurrentCamera->Eye;
    }
    if (mLerpCam)
    {
        Camera::CurrentCamera->Eye = calc::Lerp(mBaseCamPos, object->GameTransform->Position - Camera::CurrentCamera->LookAt, 1 - mCamTravelTime);

        mCamTravelTime -= Time::DeltaTime;

        if (mCamTravelTime <= 0)
        {
            mLerpCam = false;
        }
    }
}


bool SceneGraph::RightClickObject(Object* object)
{
    if (ImGui::BeginPopupContextWindow(std::to_string(object->mUUID).c_str(), ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_NoOpenOverExistingPopup) ||
        ImGui::BeginPopupContextItem(std::to_string(object->mUUID).c_str(), ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
    {
        mSelectedObject = object;
        bool success = false;
        if (ImGui::BeginMenu(object->mUUID == Object::mRoot->mUUID ? "Add Object" : "Add Child"))
        {
            if (ImGui::MenuItem("Empty"))
            {
                Object* newObject = SceneManager::ActualScene->AddObject(object, "Empty");
                ClickSelectObject(newObject);
                ImGui::CloseCurrentPopup();
                success = true;
            }
            if (ImGui::BeginMenu("Basic Shape"))
            {
                if (ImGui::MenuItem("Cube"))
                {
                    Object* newObject = SceneManager::ActualScene->AddObject(object, "Cube");
                    //Add Model
                    newObject->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/cube.obj");
                    ClickSelectObject(newObject);
                    ImGui::CloseCurrentPopup();
                    success = true;
                }
                if (ImGui::MenuItem("Sphere"))
                {
                    Object* newObject = SceneManager::ActualScene->AddObject(object, "Sphere");
                    //Add Model
                    newObject->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/sphere.obj");
                    ClickSelectObject(newObject);
                    ImGui::CloseCurrentPopup();
                    success = true;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Model With Collider"))
            {
                if (ImGui::MenuItem("Cube"))
                {
                    Object* newObject = SceneManager::ActualScene->AddObject(object, "Cube");
                    //Add Model and Collider
                    newObject->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/cube.obj");
                    newObject->AddComponent<BoxCollider>(newObject->GameTransform->GetPosition(), newObject->GameTransform->GetRotationQuat(), newObject->GameTransform->Scale);
                    ClickSelectObject(newObject);
                    ImGui::CloseCurrentPopup();
                    success = true;
                }
                if (ImGui::MenuItem("Sphere"))
                {
                    Object* newObject = SceneManager::ActualScene->AddObject(object, "Sphere");
                    //Add Model and Collider
                    newObject->AddComponent<ModelRenderer>()->ModelObject = ResourceManager::Get<Model>("assets/sphere.obj");
                    newObject->AddComponent<CapsuleCollider>(newObject->GameTransform->GetPosition(), newObject->GameTransform->GetRotationQuat(), 1, 1);
                    ClickSelectObject(newObject);
                    ImGui::CloseCurrentPopup();
                    success = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (object->mUUID != Object::mRoot->mUUID && ImGui::Button("Rename"))
        {
            mRenamingObject = object;
            ImGui::CloseCurrentPopup();
        }
        if (object->mUUID != Object::mRoot->mUUID && ImGui::Button("Remove Object"))
        {
            ServiceLocator::Get<Renderer>()->ObjectIndex = -1;
            SceneManager::ActualScene->RemoveObject(object);
        }
        ImGui::EndPopup();
        return success;
    }
    return false;
}

void SceneGraph::BeginDragObject(Object* object)
{
    // Our buttons are both drag sources and drag targets here!
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
    {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("DragObject", &object, sizeof(Object*));

        // Display preview (could be anything, e.g. when dragging an image we could decide to display
        // the filename and a small preview of the image, etc.)
        ImGui::Text("%s", object->Name.c_str());
        ImGui::EndDragDropSource();
    }
}

bool SceneGraph::BeginDropOnObject(Object* object, bool setBefore)
{
    bool success = false;
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragObject"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Object*));
            Object* payloadObject = *(Object**)payload->Data;

            if (setBefore)
            {
                unsigned int index = 0; // find object index    
                for (Object* find : object->mParent->mChildren)
                {
                    if (find == object)
                    {
                        object->mParent->AtachChild(payloadObject, index);
                        break;
                    }
                    index++;
                }
            }
            else
            {
                payloadObject->SetParent(object);
            }
            success = true;
        }
        ImGui::EndDragDropTarget();
    }
    return success;
}

void SceneGraph::RenameObject(Object* object)
{
    if (mRenamingObject == object)
    {
        ImGui::SameLine();
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;

        char* newName = (char*)object->Name.c_str();

        ImGui::SetKeyboardFocusHere();
        ImGui::PushItemWidth(ImGui::CalcTextSize(newName).x + 5);
        if (ImGui::InputText("##label", (char*)newName, 256, flags))
        {
            object->Name = newName;
            mRenamingObject = nullptr;
        }
    }
}
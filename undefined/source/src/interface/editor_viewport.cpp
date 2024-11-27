#include "interface/editor_viewport.h"

#include <imgui/imgui.h>

#include <toolbox/calc.h>

#include <vector>

#include "world/scene_manager.h"
#include "world/gizmo.h"

#include "utils/utils.h"

#include "engine_debug/logger.h"

#include "resources/resource_manager.h"

#include "resources/texture.h"

#include "interface/interface.h"


EditorViewport::EditorViewport(Framebuffer* framebuffer, Camera* camera)
	: mFramebuffer(framebuffer), ViewportCamera(camera), mShader(ResourceManager::Get<Shader>("viewport_shader"))
{
	mEditorNumber++;
	mID = mEditorNumber;

	if (mEditorNumber == 1)
	{
		Camera::CurrentCamera = ViewportCamera;
	}
}

EditorViewport::~EditorViewport()
{
	delete mFramebuffer;
	delete ViewportCamera;
}

void EditorViewport::Init()
{
	ServiceLocator::Get<Renderer>()->SetQuad(mVBO, mEBO, mVAO);
}

void EditorViewport::InitButtonTextures()
{
	mPlayID = Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/play.png")->GetID());
	mStopID = Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/stop.png")->GetID());
	mPauseID = Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/pause.png")->GetID());
}

void EditorViewport::ShowWindow()
{
	ImGui::Begin((std::string("Editor ##") + std::to_string(mID)).c_str(), 0, SceneGizmo.GizmoWindowFlags);


	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
	{
		ServiceLocator::Get<InputManager>()->GetKeyInput("editorCameraInput")->SetIsEnabled(true);
		Camera::CurrentCamera = ViewportCamera;
	}

	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::Button("Create Editor Viewport"))
		{
			Interface::CreateEditorViewport();
			ImGui::CloseCurrentPopup();
		}
		if (Interface::EditorViewports.size() > 1)
		{
			if (ImGui::Button("Delete Editor Viewport"))
			{
				Interface::DeleteEditorViewport(mID);
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				ImGui::End();
				return;
			}
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginTable("TopBar", 2))
	{
		ImGui::TableNextColumn();
		SceneGizmo.ChangeGizmoOperation();

		ImGui::TableNextColumn();

		DisplayPlayButtons();

		ImGui::EndTable();
	}

	mWidth = ImGui::GetContentRegionAvail().x;
	mHeight = ImGui::GetContentRegionAvail().y;
	ViewportCamera->Width = mWidth;
	ViewportCamera->Height = mHeight;

	Vector2 viewportOffset;
	Vector2 viewportSize;
	int mouseX;
	int mouseY;

	SetMouseMinMaxBounds(mouseX, mouseY, viewportOffset, viewportSize);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && mouseX >= 0 && mouseY >= 0 && 
		mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
	{
		if (!ImGuizmo::IsOver())
		{
			ServiceLocator::Get<Renderer>()->ReadPixels(GetFBO_ID(), 1, mouseX, mouseY);
			Logger::Info("Pixel data = {}", ServiceLocator::Get<Renderer>()->ObjectIndex);
		}
	}

	// we get the screen position of the window
	ImVec2 screenPos = ImGui::GetCursorScreenPos();
	
	ImGui::GetWindowDrawList()->AddImage(
		Utils::IntToPointer<ImTextureID>(mFramebuffer->FramebufferTextures[0]->GetID()),
		ImVec2(screenPos.x, screenPos.y),
		ImVec2(screenPos.x + mWidth, screenPos.y + mHeight),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	int objectIndex = ServiceLocator::Get<Renderer>()->ObjectIndex;
	
	if (objectIndex >= 0 && !mIsGizmoUpdated && Camera::CurrentCamera == ViewportCamera)
	{
		SceneGizmo.DrawGizmos(ViewportCamera, SceneManager::ActualScene->Objects[ServiceLocator::Get<Renderer>()->ObjectIndex]->GameTransform);
		mIsGizmoUpdated = true;
	}
	

	ImGui::End();
}

unsigned int EditorViewport::GetFBO_ID() const
{
	return mFramebuffer->FBO_ID;
}

int EditorViewport::GetEditorID() const
{
	return mID;
}

void EditorViewport::RescaleViewport()
{
	if (mWidth <= 0 || mHeight <= 0)
	{
		return;
	}

	Matrix4x4 result;
	if (mHeight <= 0)
	{
		result = Matrix4x4::Identity();
	}
	else
	{
		float aspect = mWidth / mHeight;
		if (aspect < 1.0f)
			aspect = mHeight / mWidth;

		result = Matrix4x4::ProjectionMatrix(calc::PI / 2.0f, aspect, 0.1f, 100.0f);
		ViewportCamera->SetPerspective(result);
	}

	mFramebuffer->RescaleFramebuffer(mWidth, mHeight);

	// TODO add to Renderer
	glViewport(0, 0, (GLsizei)mWidth, (GLsizei)mHeight);
}

void EditorViewport::SetMouseMinMaxBounds(int& mouseX, int& mouseY, Vector2& viewportOffset, Vector2& viewportSize)
{
	viewportOffset.x = ImGui::GetCursorPos().x;
	viewportOffset.y = ImGui::GetCursorPos().y;

	//min and max size of the framebuffer and mouse pos
	ImVec2 minBound = ImGui::GetWindowPos();
	minBound.x += viewportOffset.x;
	minBound.y += viewportOffset.y;
	ImVec2 maxBound = { minBound.x + mWidth, minBound.y + mHeight };
	mViewportBounds[0] = { minBound.x, minBound.y };
	mViewportBounds[1] = { maxBound.x, maxBound.y };
	auto [mx, my] = ImGui::GetMousePos();
	mx -= mViewportBounds[0].x;
	my -= mViewportBounds[0].y;
	viewportSize = mViewportBounds[1] - mViewportBounds[0];
	my = viewportSize.y - my;

	mouseX = (int)mx;
	mouseY = (int)my;
}

void EditorViewport::DisplayPlayButtons()
{
	if (ImGui::ImageButton("##play", SceneManager::IsScenePlaying ? mStopID : mPlayID, mIconSize))
	{
		if (!SceneManager::IsScenePlaying)
		{
			SceneManager::Start();
		}
		else
		{
			SceneManager::Reload();
		}
		SceneManager::IsScenePlaying = !SceneManager::IsScenePlaying;
	}

	ImGui::SameLine();

	if (ImGui::ImageButton("##pause", mPauseID, mIconSize, ImVec2(0, 0), ImVec2(1, 1), SceneManager::IsScenePaused ? ImVec4(0.5f, 0.5f, 0.5f, 1) : ImVec4()))
	{
		SceneManager::IsScenePaused = !SceneManager::IsScenePaused;
	}
}

void EditorViewport::SetIsGizmoUpdated(bool value)
{
	mIsGizmoUpdated = value;
}

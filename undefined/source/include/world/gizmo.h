#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

class Camera;
class Transform;

/// <summary>
/// Class for the Gizmo 
/// </summary>
class Gizmo
{
public:
	/// <summary>
	/// Default Constructor of Gizmo
	/// </summary>
	Gizmo();
	/// <summary>
	/// Draw the Gizmo and modify it
	/// </summary>
	/// <param name="camera">: Camera where you want to display the gizmo</param>
	/// <param name="transform">: Transform of the object selected</param>
	void DrawGizmos(Camera* camera, Transform* transform);

	/// <summary>
	/// Change the gizmo mode (e.g : ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::WORLD, ...)
	/// </summary>
	void ChangeGizmoOperation();

	/// <summary>
	/// Flags for  the window
	/// </summary>
	static inline ImGuiWindowFlags GizmoWindowFlags;

	/// <summary>
	/// Gizmo operation (e.g : ImGuizmo::TRANSLATE, ImGuizmo::ROTATE, ImGuizmo::SCALE, ...)
	/// </summary>
	static inline ImGuizmo::OPERATION CurrentGizmoOperation;
	/// <summary>
	/// Gizmo mode (e.g : ImGuizmo::WORLD, ImGuizmo::LOCAL)
	/// </summary>
	static inline ImGuizmo::MODE CurrentGizmoMode;

public:
	/// <summary>
	/// Draw a debug grid in front of the scene
	/// </summary>
	/// <param name="camera">: Camera where you want to display the grid</param>
	static void DrawGrid(Camera* camera);
};
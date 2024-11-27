#pragma once 

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <filesystem>
#include <vector>

#include "utils/flag.h"
#include "world/scene.h"
#include "interface/editor_viewport.h"

/// <summary>
/// A static class to manage the Interface
/// </summary>
class Interface
{
	STATIC_CLASS(Interface)

public:
	/// <summary>
	/// Create a new editor viewport
	/// </summary>
	UNDEFINED_ENGINE static void CreateEditorViewport();
	/// <summary>
	/// Delete an editor viewport by is ID
	/// </summary>
	/// <param name="ID">: ID of the editor viewport</param>
	UNDEFINED_ENGINE static void DeleteEditorViewport(int ID);

	/// <summary>
	/// Init the interface sytem
	/// </summary>
	static void Init();
	/// <summary>
	/// Update the interface system
	/// </summary>
	static void Update();
	/// <summary>
	/// Render the interface system
	/// </summary>
	static void Render();
	/// <summary>
	/// Delete the interface system
	/// </summary>
	static void Delete();

	/// <summary>
	/// std::vector of pointers of EditorViewport that store all of the editor viewports
	/// </summary>
	static inline std::vector<EditorViewport*> EditorViewports;

private:
	/// <summary>
	/// 
	/// </summary>
	static void NewFrame();
	/// <summary>
	/// 
	/// </summary>
	static void BeginDockSpace();
};
#include "world/gizmo.h"

#include "camera/camera.h"

#include "world/transform.h"

Gizmo::Gizmo()
{
	GizmoWindowFlags = 0;

    CurrentGizmoMode = ImGuizmo::WORLD;
    CurrentGizmoOperation = ImGuizmo::TRANSLATE;
}

void Gizmo::DrawGizmos(Camera* camera, Transform* transform)
{
    Matrix4x4 matrix = (CurrentGizmoMode == ImGuizmo::WORLD) ? transform->WorldMatrix() : transform->LocalMatrix();
    matrix.Transpose();
    Matrix4x4 view = camera->GetView();
    view = view.Transpose();
    Matrix4x4 proj = camera->GetProjection();
    proj = proj.Transpose();

    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();

    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    ImGuizmo::Manipulate(&view[0].x, &proj[0].x, CurrentGizmoOperation, CurrentGizmoMode, &matrix[0].x, NULL, NULL, NULL, NULL);
    
    if (CurrentGizmoMode == ImGuizmo::WORLD)
    {
        transform->SetWorldMatrix(matrix.Transpose());
    }
    else if (CurrentGizmoMode == ImGuizmo::LOCAL)
    {
        transform->SetLocalMatrix(matrix.Transpose());
    }
}

void Gizmo::ChangeGizmoOperation()
{
    if (ImGui::RadioButton("Translate", CurrentGizmoOperation == ImGuizmo::TRANSLATE))
    {
        CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", CurrentGizmoOperation == ImGuizmo::ROTATE))
    {
        CurrentGizmoOperation = ImGuizmo::ROTATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", CurrentGizmoOperation == ImGuizmo::SCALE))
    {
        CurrentGizmoOperation = ImGuizmo::SCALE;
    }

    if (ImGui::RadioButton("Local", CurrentGizmoMode == ImGuizmo::LOCAL))
    {
        CurrentGizmoMode = ImGuizmo::LOCAL;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("World", CurrentGizmoMode == ImGuizmo::WORLD))
    {
        CurrentGizmoMode = ImGuizmo::WORLD;
    }
}

void Gizmo::DrawGrid(Camera* camera)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    GizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

    Matrix4x4 view = camera->GetView();
    view = view.Transpose();
    Matrix4x4 proj = camera->GetProjection();
    proj = proj.Transpose();

    Matrix4x4 identity = Matrix4x4::Identity();

    ImGuizmo::SetDrawlist();

    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();

    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    ImGuizmo::DrawGrid(&view[0].x, &proj[0].x, &identity[0].x, 10000);

}

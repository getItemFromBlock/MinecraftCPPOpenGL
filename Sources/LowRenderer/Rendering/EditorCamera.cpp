#include "LowRenderer/Rendering/EditorCamera.hpp"

#include <ImGUI/imgui.h>

using namespace Core::Maths;

void LowRenderer::Rendering::EditorCamera::RenderGUI()
{
    Camera::RenderGUI();
    ImGui::DragFloat3("Camera Position", &position.x, 0.1f);
    ImGui::DragFloat3("Camera Rotation", &rotation.x, 0.1f);
    ImGui::DragFloat("Camera Distance", &distance, 0.1f);
    ImGui::DragFloat("Movement Speed", &MovementSpeed, 0.1f);
    ImGui::DragFloat("Rotation Speed", &RotationSpeed, 0.1f);
}

void LowRenderer::Rendering::EditorCamera::Update(const Core::App::Inputs& inputs, const float deltaTime)
{
    if (inputs.mouseCaptured)
    {
        distance = 5.0f;
        // Update rotation.
        rotation = rotation + Vec3(-inputs.deltaMouse.x, inputs.deltaMouse.y, 0) * RotationSpeed;
        rotation = Vec3(Util::mod(rotation.x, 360), Util::clamp(rotation.y, -90.0f, 90.0f), Util::mod(rotation.z, 360));
    }

    // Update distance.
    Mat4 Rot = Core::Maths::Mat4::CreateRotationMatrix(Vec3(-rotation.y, rotation.x, rotation.z));
    focus = position - (Rot * Core::Maths::Vec3(0, 0, distance)).getVector();
    aspect_ratio = inputs.ScreenSize.x * 1.0f / inputs.ScreenSize.y;
    Resolution = inputs.ScreenSize;
    deltaUp = (Rot * up).getVector();
}

void LowRenderer::Rendering::EditorCamera::Update(const Core::App::Inputs& inputs, const Vec3& posIn, CameraViewMode mode, const float deltaTime)
{
    if (inputs.mouseCaptured)
    {
        distance = 5.0f;
        // Update rotation.
        rotation = rotation + Vec3(-inputs.deltaMouse.x, inputs.deltaMouse.y, 0) * RotationSpeed;
        rotation = Vec3(Util::mod(rotation.x, 360), Util::clamp(rotation.y, -90.0f, 90.0f), Util::mod(rotation.z, 360));
    }

    // Update distance.
    Mat4 Rot = Core::Maths::Mat4::CreateRotationMatrix(Vec3(-rotation.y, rotation.x, rotation.z));
    switch (mode)
    {
    case CameraViewMode::DEFAULT:
        position = posIn;
        focus = position - (Rot * Core::Maths::Vec3(0, 0, distance)).getVector();
        break;
    case CameraViewMode::BACK:
        focus = posIn;
        position = focus + (Rot * Core::Maths::Vec3(0, 0, distance)).getVector();
        break;
    case CameraViewMode::FRONT:
        focus = posIn;
        position = focus - (Rot * Core::Maths::Vec3(0, 0, distance)).getVector();
        break;
    default:
        break;
    }
    aspect_ratio = inputs.ScreenSize.x * 1.0f / inputs.ScreenSize.y;
    Resolution = inputs.ScreenSize;
    deltaUp = (Rot * up).getVector();
}

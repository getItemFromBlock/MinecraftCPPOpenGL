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
        rotation = rotation + Vec3D(-inputs.deltaMouse.x, inputs.deltaMouse.y, 0) * RotationSpeed;
        rotation = Vec3D(Util::mod(rotation.x, 360), Util::cut(rotation.y, -90.0f, 90.0f), Util::mod(rotation.z, 360));
        /*
        // Update focus.
        float dSpeed = deltaTime * MovementSpeed * (inputs.shift ? 5.0f : (inputs.control ? 0.2f : 1.0f));
        Vec3D delta = Vec3D(dSpeed * inputs.right - dSpeed * inputs.left, dSpeed * inputs.up - dSpeed * inputs.down, dSpeed * inputs.backward - dSpeed * inputs.forward) / 20;
        focus = focus + Vec3D(cosf(Util::toRadians(rotation.x)) * delta.x + sinf(Util::toRadians(rotation.x)) * delta.z, delta.y, -sinf(Util::toRadians(rotation.x)) * delta.x + cosf(Util::toRadians(rotation.x)) * delta.z);
        // Update Position
        */
    }

    // Update distance.
    Mat4D Rot = Core::Maths::Mat4D::CreateRotationMatrix(Vec3D(-rotation.y, rotation.x, rotation.z));
    focus = position - (Rot * Core::Maths::Vec3D(0, 0, distance)).getVector();//Vec3D(sinf(Util::toRadians(rotation.x)) * cosf(Util::toRadians(rotation.y)), sinf(Util::toRadians(rotation.y)), cosf(Util::toRadians(rotation.x)) * cosf(Util::toRadians(rotation.y)))* (distance == 0.0f ? 0.001f : distance);
    aspect_ratio = inputs.ScreenSize.x * 1.0f / inputs.ScreenSize.y;
    Resolution = inputs.ScreenSize;
    deltaUp = (Rot * up).getVector();
}
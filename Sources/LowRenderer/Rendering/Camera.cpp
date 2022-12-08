#include "LowRenderer/Rendering/Camera.hpp"

#include <ImGUI/imgui.h>

using namespace Core::Maths;

LowRenderer::Rendering::Camera::Camera()
{
	position = Vec3(0, 0, 1);
	focus = Vec3(0, 0, 0);
	up = Vec3(0, 1, 0);
    rotation = Vec3();

	fov = 60;
    nearPlane = 0.1f;
    farPlane = 600.0f;
	aspect_ratio = 4.0f / 3.0f;
}

LowRenderer::Rendering::Camera::~Camera()
{
}


Mat4 LowRenderer::Rendering::Camera::GetViewMatrix()
{
    Mat4 temp;
    Vec3 z = (position - focus).unitVector();
    Vec3 x = deltaUp.crossProduct(z).unitVector();
    Vec3 y = z.crossProduct(x);
    Vec3 delta = Vec3(-x.dotProduct(position), -y.dotProduct(position), -z.dotProduct(position));
    for (int i = 0; i < 3; i++)
    {
        temp.at(i, 0) = x[i];
        temp.at(i, 1) = y[i];
        temp.at(i, 2) = z[i];
        temp.at(3, i) = delta[i];
    }
    temp.at(3, 3) = 1;
    return temp;
}

Mat4 LowRenderer::Rendering::Camera::GetProjectionMatrix()
{
    float s = 1.0f / tanf(Util::toRadians(fov / 2.0f));
    float param1 = -(farPlane + nearPlane) / (farPlane - nearPlane);
    float param2 = -(2 * nearPlane * farPlane) / (farPlane - nearPlane);
    Mat4 out;
    out.at(0, 0) = s / aspect_ratio;
    out.at(1, 1) = s;
    out.at(2, 2) = param1;
    out.at(2, 3) = -1;
    out.at(3, 2) = param2;
    return out;
}

Mat4 LowRenderer::Rendering::Camera::GetOrthoMatrix()
{
    float s = 1.0f / fov;
    float param1 = -2 / (farPlane - nearPlane);
    float param2 = -(farPlane + nearPlane) / (farPlane - nearPlane);
    Mat4 out;
    out.at(0, 0) = s / aspect_ratio;
    out.at(1, 1) = s;
    out.at(2, 2) = param1;
    out.at(3, 3) = 1;
    out.at(3, 2) = param2;
    return out;
}

void LowRenderer::Rendering::Camera::RenderGUI()
{
    ImGui::DragFloat("Camera FOV", &fov, 0.1f);
    ImGui::DragFloat("Near Plane", &nearPlane, 0.1f);
    ImGui::DragFloat("Far Plane", &farPlane, 0.1f);
    ImGui::DragFloat3("Camera Up Vector", &up.x, 0.1f);
}

void LowRenderer::Rendering::Camera::Update(const Core::App::Inputs& inputs, const float deltaTime)
{
    focus = position - Vec3(sinf(Util::toRadians(rotation.x)) * cosf(Util::toRadians(rotation.y)), sinf(Util::toRadians(rotation.y)), cosf(Util::toRadians(rotation.x)) * cosf(Util::toRadians(rotation.y)));
    aspect_ratio = inputs.ScreenSize.x * 1.0f / inputs.ScreenSize.y;
    Resolution = inputs.ScreenSize;
    deltaUp = (Mat4::CreateYRotationMatrix(rotation.x + 180) * Mat4::CreateXRotationMatrix(rotation.y) * Mat4::CreateZRotationMatrix(rotation.z) * up).getVector();
}

void LowRenderer::Rendering::Camera::Update(IVec2 resolution, Vec3 pos, Vec3 forward, Vec3 upIn)
{
    aspect_ratio = resolution.x * 1.0f / resolution.y;
    Resolution = resolution;
    position = pos;
    focus = position + forward;
    deltaUp = upIn;
}

void LowRenderer::Rendering::Camera::Update(Core::Maths::IVec2 resolution, Core::Maths::Vec3 pos, Core::Maths::Vec3 foc)
{
    aspect_ratio = resolution.x * 1.0f / resolution.y;
    Resolution = resolution;
    position = pos;
    focus = foc;
    Core::Maths::Vec3 forward = (focus - position);
    Core::Maths::Vec3 tmpRight = up.crossProduct(forward);
    if (tmpRight.lengthSquared() < 0.001f) tmpRight = Core::Maths::Vec3(1, 0, 0);
    else tmpRight = tmpRight.unitVector();
    deltaUp = forward.crossProduct(tmpRight);
}

#pragma once

#include "Core/Maths/Maths.hpp"

#include "Core/App/Input.hpp"

namespace LowRenderer::Rendering
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		Core::Maths::Vec3 position;
		Core::Maths::Vec3 focus;
		Core::Maths::Vec3 up;
		Core::Maths::Vec3 rotation;
		float fov = 60;
		float nearPlane = 0.1f;
		float farPlane = 600.0f;

		virtual void RenderGUI();
		virtual void Update(const Core::App::Inputs& inputs, const float deltaTime);
		virtual void Update(Core::Maths::IVec2 resolution, Core::Maths::Vec3 position, Core::Maths::Vec3 forward, Core::Maths::Vec3 up);
		virtual void Update(Core::Maths::IVec2 resolution, Core::Maths::Vec3 position, Core::Maths::Vec3 focus);
		Core::Maths::Mat4 GetViewMatrix();
		Core::Maths::Mat4 GetProjectionMatrix();
		Core::Maths::Mat4 GetOrthoMatrix();

		Core::Maths::IVec2 GetResolution() const { return Resolution; }

	protected:
		Core::Maths::IVec2 Resolution;
		Core::Maths::Vec3 deltaUp;
		float aspect_ratio;
	};
}
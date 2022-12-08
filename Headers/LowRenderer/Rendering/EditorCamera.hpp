#pragma once

#include "LowRenderer/Rendering/Camera.hpp"

enum class CameraViewMode : char
{
	DEFAULT = 0,
	BACK,
	FRONT,
};

namespace LowRenderer::Rendering
{
	class EditorCamera : public Camera
	{
	public:
		float distance = 5.0f;

		float MovementSpeed = 40.0f;
		float RotationSpeed = 0.12f;

		void RenderGUI() override;
		void Update(const Core::App::Inputs& inputs, const float deltaTime) override;
		void Update(const Core::App::Inputs& inputs, const Core::Maths::Vec3& posIn, CameraViewMode mode, const float deltaTime);
	private:

	};
}
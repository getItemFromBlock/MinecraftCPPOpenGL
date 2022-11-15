#pragma once

#include "LowRenderer/Rendering/RenderCamera.hpp"

namespace LowRenderer::Rendering
{
	class ReflectionCamera : public RenderCamera
	{
	public:
		ReflectionCamera();
		~ReflectionCamera();

		void Update( std::vector<RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime) override;
		void DeleteCamera() override;

	protected:
		Core::Maths::Vec3D PlaneNormal;
		Core::Maths::Vec3D PlaneRight;
		Core::Maths::Vec3D PlaneUp;
	};
}
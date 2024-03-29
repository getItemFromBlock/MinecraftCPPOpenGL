#pragma once

#include <vector>

#include "Core/Maths/Maths.hpp"

#include "Resources/ShaderManager.hpp"
#include "Resources/TextureManager.hpp"
#include "LowRenderer/Lightning/ShadowMapBuffer.hpp"

#define DIRECTIONAL_SIZE 13llu
#define POINT_SIZE 15llu
#define SPOT_SIZE 20llu

namespace Core::DataStructure
{
	class SceneNode;
}
namespace LowRenderer
{
	namespace Lightning
	{
		class LightManager;
	}
	namespace Rendering
	{
		class RenderCamera;
	}
}

enum class ShadowUpdateType
{
	None,
	Baked,
	Realtime,
};

namespace LowRenderer
{
	class Light
	{
	public:
		Light() {}
		~Light() {}
		Core::Maths::Vec3 AmbientLight = Core::Maths::Vec3(1);
		Core::Maths::Vec3 DiffuseLight = Core::Maths::Vec3();
		Core::Maths::Vec3 SpecularLight = Core::Maths::Vec3();
		float Smoothness = 1.0f;
		virtual float* GetValues() = 0;
		virtual void DeleteLight() = 0;
		virtual void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime);
	protected:
		bool inited = false;
		LowRenderer::Lightning::ShadowMapBuffer* shadowMap = nullptr;
		ShadowUpdateType ShadowType = ShadowUpdateType::None;
		float nearPlane = 0.1f;
		float farPlane = 200.0f;
		friend class Core::DataStructure::SceneNode;
		friend class LowRenderer::Lightning::LightManager;
	};

	class DirectionalLight : public Light
	{
	protected:
		Core::Maths::Vec3 GlobalDirection = Core::Maths::Vec3(0, -1, 0);
	public:
		DirectionalLight() {}
		~DirectionalLight() {}
		Core::Maths::Vec3 Rotation = Core::Maths::Vec3(0, 0, 0);
		void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime) override;
		virtual float* GetValues() override;
		virtual void DeleteLight() override;

		friend class LowRenderer::Lightning::LightManager;
	};

	class PointLight : public Light
	{
	protected:
		Core::Maths::Vec3 Position = Core::Maths::Vec3(1);
	public:
		PointLight() {}
		~PointLight() {}
		Core::Maths::Vec2 Attenuation = Core::Maths::Vec2(0, 0);
		virtual void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime) override;
		virtual float* GetValues() override;
		virtual void DeleteLight() override;
		friend class Core::DataStructure::SceneNode;
		friend class LowRenderer::Lightning::LightManager;
	};

	class SpotLight : public PointLight
	{
	protected:
		Core::Maths::Vec3 GlobalDirection = Core::Maths::Vec3(0, -1, 0);
		Core::Maths::Vec3 GlobalUp = Core::Maths::Vec3(0, 1, 0);
	public:
		SpotLight() {}
		~SpotLight() {}
		Core::Maths::Vec3 Rotation = Core::Maths::Vec3(0, 0, 0);
		float Spotangle = 0.5f;
		float Spotratio = 0.01f;
		void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime) override;
		virtual float* GetValues() override;
		virtual void DeleteLight() override;
		friend class Core::DataStructure::SceneNode;
		friend class LowRenderer::Lightning::LightManager;
	};
}
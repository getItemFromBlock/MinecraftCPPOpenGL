#pragma once

#include "LowRenderer/Rendering/Camera.hpp"
#include "LowRenderer/Rendering/FrameBuffer.hpp"

namespace Resources
{
	class ModelHolder;
	class ShaderManager;
	class MeshManager;
	class ResourceManager;
	class MaterialManager;
	class TextureManager;
}

namespace LowRenderer
{
	namespace Lightning
	{
		class LightManager;
	}
	namespace Rendering
	{
		class RenderCamera : public Camera
		{
		public:
			RenderCamera();
			~RenderCamera();

			virtual void Update(std::vector<RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime);
			virtual void DeleteCamera();
			void RefreshResolution() { Resolution = tmpResolution; }

			Core::Maths::IVec2 tmpResolution = Core::Maths::IVec2(256, 256);
			bool ClearBuffer = true;
			Core::Maths::Vec3 ClearColor = Core::Maths::Vec3();
		protected:
			char UUID[17] = "";
			LowRenderer::Rendering::FrameBuffer* buffer = nullptr;
			TextureFilterType Filter = TextureFilterType::Linear;
			TextureWrapType Wrap = TextureWrapType::ClampToEdge;
		};
	}
}
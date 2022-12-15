#pragma once

#include <vector>

#include "Resources/Texture.hpp"
#include "Resources/Font.hpp"
#include "Resources/ResourceManager.hpp"
#include "TextureAtlas.hpp"

namespace LowRenderer
{
	namespace Lightning
	{
		class ShadowMapBuffer;
	}
	namespace Rendering
	{
		class FrameBuffer;
	}
}

namespace Resources
{
	struct TextureData
	{
		unsigned char* data = nullptr;
		int width = 0;
		int height = 0;
		std::string name;
	};
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();
		size_t CreateTexture(ResourceManager* manager, const char* path, unsigned int textureParam);
		size_t CreateTexture(ResourceManager* manager, const char* path);
		size_t CreateFont(ResourceManager* manager, const char* path, unsigned int textureParam);
		size_t AddShadowMap(LowRenderer::Lightning::ShadowMapBuffer* in);
		size_t AddFrameBuffer(LowRenderer::Rendering::FrameBuffer* in);
		void ClearShadowMaps(ResourceManager* manager);
		std::vector<Texture*> GetTextures();
		std::vector<Texture*> GetTextures(const char* filter);
		void LoadAtlas(Resources::TextureAtlas& atlas);
		void LoadTextures(ResourceManager* manager);
	private:
		std::vector<Texture*> textures;
		std::vector<LowRenderer::Lightning::ShadowMapBuffer*> shadowMaps;
		bool IsFont = false;
	};
}
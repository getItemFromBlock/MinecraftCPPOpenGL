#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Core/Maths/Maths.hpp"

namespace Resources
{
	class TextureAtlas
	{
	public:
		TextureAtlas();
		TextureAtlas(Core::Maths::Int2D size, const std::string& name = "texture_atlas");
		~TextureAtlas();

		void FillRegion(Core::Maths::Int2D position, Core::Maths::Int2D fillSize, const Core::Maths::UChar4D* data, int dataWidth);

		void Load();
		void UnLoad();
		unsigned int GetID() { return Tex_ID; }
		Core::Maths::UChar4D* GetData() { return textureData; }

		void PushTextureIndex(std::string name, Core::Maths::Int2D pos);

		Core::Maths::Vec2D GetTexture(std::string name);
		Core::Maths::Vec2D GetTextureSize();

	private:
		Core::Maths::Int2D size;
		unsigned int Tex_ID = 0;
		unsigned int Sampler_ID = 0;
		bool loaded = false;
		std::string Name = "Texture";
		std::unordered_map<std::string, Core::Maths::Vec2D> textureIndex;
		Core::Maths::UChar4D* textureData = nullptr;
	};
}
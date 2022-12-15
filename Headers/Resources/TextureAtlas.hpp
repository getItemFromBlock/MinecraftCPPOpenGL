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
		TextureAtlas(Core::Maths::IVec2 size, const std::string& name = "texture_atlas");
		~TextureAtlas();

		void FillRegion(Core::Maths::IVec2 position, Core::Maths::IVec2 fillSize, const Core::Maths::Color4* data, int dataWidth);

		void Load();
		void UnLoad();
		unsigned int GetID() { return Tex_ID; }
		Core::Maths::Color4* GetData() { return textureData; }

		void PushTextureIndex(std::string name, Core::Maths::IVec2 pos);

		Core::Maths::Vec2 GetTexture(std::string name);
		Core::Maths::Vec2 GetTextureSize();

		Core::Maths::IVec2 GetTextureResolution();

	private:
		Core::Maths::IVec2 size;
		unsigned int Tex_ID = 0;
		unsigned int Sampler_ID = 0;
		bool loaded = false;
		std::string Name = "Texture";
		std::unordered_map<std::string, Core::Maths::Vec2> textureIndex;
		Core::Maths::Color4* textureData = nullptr;
	};
}
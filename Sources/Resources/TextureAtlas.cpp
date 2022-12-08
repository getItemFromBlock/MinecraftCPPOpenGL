#include "Resources/TextureAtlas.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace Resources;

TextureAtlas::TextureAtlas()
{
}

TextureAtlas::TextureAtlas(Core::Maths::IVec2 s, const std::string& n)
{
	Assert(s.x > 0 && s.y > 0);
	Name = n;
	size = s;
	textureData = new Core::Maths::Color4[(uint64_t)size.x * size.y];
}

TextureAtlas::~TextureAtlas()
{
	if (textureData) delete[] textureData;
}

void TextureAtlas::Load()
{
	if (!loaded) glGenTextures(1, &Tex_ID);
	glBindTexture(GL_TEXTURE_2D, Tex_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (!loaded) glGenSamplers(1, &Sampler_ID);
	glSamplerParameteri(Sampler_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(Sampler_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(Sampler_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(Sampler_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glSamplerParameterf(Sampler_ID, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.f);

	glBindTextureUnit(Tex_ID, Tex_ID);
	glBindSampler(Tex_ID, Sampler_ID);
	loaded = true;
}

void TextureAtlas::UnLoad()
{
	if (!loaded) return;
	glDeleteTextures(1, &Tex_ID);
	glDeleteSamplers(1, &Sampler_ID);
	loaded = false;
}

void Resources::TextureAtlas::PushTextureIndex(std::string name, Core::Maths::IVec2 pos)
{
	Core::Maths::Vec2 rPos = Core::Maths::Vec2(pos) / (float)size.x;
	textureIndex.emplace(name, rPos);
}

Core::Maths::Vec2 Resources::TextureAtlas::GetTexture(std::string name)
{
	auto result = textureIndex.find(name.append(".png"));
	if (result == textureIndex.end()) return textureIndex.find("block/debug.png")->second;
	return result->second;
}

Core::Maths::Vec2 Resources::TextureAtlas::GetTextureSize()
{
	return Core::Maths::Vec2(16*1.0f/size.x, 16 * 1.0f / size.y);
}

void Resources::TextureAtlas::FillRegion(Core::Maths::IVec2 position, Core::Maths::IVec2 fillSize, const Core::Maths::Color4* data, int dataWidth)
{
	if (position.x < 0 || fillSize.x <= 0 || position.x + fillSize.x > size.x ||
		position.y < 0 || fillSize.y <= 0 || position.y + fillSize.y > size.y ||
		!textureData) return;

	for (size_t i = 0; i < fillSize.y; i++)
	{
		size_t deltaA = (i + position.y) * size.x + position.x;
		size_t deltaB = i * dataWidth;
		std::memcpy(textureData + deltaA, data + deltaB, sizeof(Core::Maths::Color4) * fillSize.x);
	}
}

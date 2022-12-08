#include "Resources/TextureManager.hpp"

#include <filesystem>
namespace fs = std::filesystem;
#include <string>

#include "Core/DataStructure/NameSearcher.hpp"
#include "LowRenderer/Lightning/ShadowMapBuffer.hpp"
#include "LowRenderer/Rendering/FrameBuffer.hpp"

using namespace Resources;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

size_t TextureManager::CreateTexture(ResourceManager* manager, const char* path)
{
	std::string path2;
	for (size_t c = 0; path[c] != 0; c++)
	{
		if (path[c] == '/') path2.append("\\\\");
		else path2 += path[c];
	}
	Texture* tex;
	if (IsFont) tex = dynamic_cast<Texture*>(manager->Get<Font>(path2.c_str()));
	else tex = manager->Get<Texture>(path2.c_str());
	if (tex)
	{
		for (size_t i = 0; i < textures.size(); i++)
		{
			if (textures[i] == tex)
			{
				return i;
			}
		}
	}
	else
	{
		if (IsFont)
		{
			tex = dynamic_cast<Texture*>(manager->Create<Font>(path));
			IsFont = false;
		}
		else tex = manager->Create<Texture>(path);
	}
	textures.push_back(tex);
	return textures.size() - 1llu;
}
size_t TextureManager::AddShadowMap(LowRenderer::Lightning::ShadowMapBuffer* in)
{
	shadowMaps.push_back(in);
	return shadowMaps.size() - 1llu;
}

size_t TextureManager::AddFrameBuffer(LowRenderer::Rendering::FrameBuffer* in)
{
	textures.push_back((Texture*)in);
	return textures.size() - 1llu;
}

size_t TextureManager::CreateTexture(ResourceManager* manager, const char* path, unsigned int textureParam)
{
	Texture::SetFilterType(textureParam);
	return CreateTexture(manager, path);
}

size_t Resources::TextureManager::CreateFont(ResourceManager* manager, const char* path, unsigned int textureParam)
{
	Texture::SetFilterType(textureParam);
	IsFont = true;
	return CreateTexture(manager, path);
}

std::vector<Texture*> TextureManager::GetTextures()
{
	return textures;
}

std::vector<Texture*> TextureManager::GetTextures(const char* filter)
{
	if (!filter || filter[0] == 0) return GetTextures();
	return Core::DataStructure::NameSearcher::FindElementsPtr<Texture>(textures, filter);
}

void TextureManager::ClearShadowMaps(ResourceManager* manager)
{
	for (size_t n = 0; n < shadowMaps.size(); n++)
	{
		manager->Delete(shadowMaps[n]->GetPath());
	}
	shadowMaps.clear();
}

void Resources::TextureManager::LoadAtlas(Resources::TextureAtlas& atlas)
{
	std::string path = "Resources/textures/";
	std::vector<std::string> files;
	std::vector<std::string> exts;
	for (const auto& entry : fs::recursive_directory_iterator(path))
	{
		files.push_back(entry.path().generic_string());
		exts.push_back(entry.path().extension().generic_string());
	}
	std::vector<TextureData> textures;
    for (size_t i = 0; i < files.size(); i++)
    {
        if (!exts[i].c_str()[0]) continue;
        else if (!exts[i].compare(".png") || !exts[i].compare(".jpg"))
        {
			textures.push_back(TextureData());
			if (!Texture::LoadImage(files[i].c_str(), textures.back()))
			{
				textures.pop_back();
			}
			textures.back().name = files[i];
        }
        else
        {
            LOG("%s:%s", files[i].c_str(), exts[i].c_str());
        }
    }
	Core::Maths::IVec2 size = Core::Maths::IVec2(16, 16);
	int index = 0;
	for (size_t i = 0; i < textures.size(); i++)
	{
		if (i >= 4096) break;
		TextureData& tmp = textures.at(i);
		if (tmp.width % 16 != 0 || tmp.height % 16 != 0) LOG("Bruh %d : [%s; %s]", tmp.width, tmp.height, tmp.name.c_str());
		atlas.PushTextureIndex(tmp.name.substr(19), Core::Maths::IVec2(16 * (index % 64), 16 * (index / 64)));
		for (int k = 0; k < tmp.width / 16; k++)
			for (int n = (tmp.height / 16 )-1; n >= 0; n--)
			{
				Core::Maths::IVec2 pos = Core::Maths::IVec2(16 * (index % 64), 16 * (index / 64));
				atlas.FillRegion(pos, size, ((Core::Maths::Color4*)textures.at(i).data)+(n*(tmp.width/16)*256+k*16), tmp.width);
				index++;
			}
	}
	for (size_t i = 0; i < textures.size(); i++) Resources::Texture::FreeImage(textures.at(i));
}
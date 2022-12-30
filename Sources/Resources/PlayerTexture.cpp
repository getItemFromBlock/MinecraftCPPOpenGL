#include "Resources/PlayerTexture.hpp"
#include "Core/Debug/Log.hpp"

#include <STB_Image/stb_image.h>

Resources::PlayerTexture::PlayerTexture()
{
	sizeX = 64;
	sizeY = 64;
}

Resources::PlayerTexture::~PlayerTexture()
{
}

void Resources::PlayerTexture::Load(const char* playerName)
{
	fullPath = playerName;
	if (loaded.Load()) return;
	int max = (int)fullPath.substr(15).copy(Name, 255);
	Name[max] = 0;

	// TODO: request data from server, load texture, save texture to disk

	//stbi_set_flip_vertically_on_load(true);
	int nrChannels = 0;
	int x = 0;
	int y = 0;
	data = stbi_load("Resources/textures/player/Steve.png", &x, &y, &nrChannels, 4);
	if (!data)
	{
		LOGRAW("ERROR   : Could not load file Resources/textures/player/Steve.png: ");
		LOG(stbi_failure_reason());
	}
}

void Resources::PlayerTexture::UnLoad()
{
	DeleteData();
	Texture::UnLoad();
	this->~PlayerTexture();
}

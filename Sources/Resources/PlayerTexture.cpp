#include "Resources/PlayerTexture.hpp"

Resources::PlayerTexture::PlayerTexture()
{
}

Resources::PlayerTexture::~PlayerTexture()
{
}

void Resources::PlayerTexture::Load(const char* playerName)
{
	fullPath = playerName;
	if (loaded.Load()) return;
	int max = fullPath.substr(15).copy(Name, 255);
	Name[max] = 0;

	// TODO: fill data
}

void Resources::PlayerTexture::UnLoad()
{
	DeleteData();
	Texture::UnLoad();
	this->~PlayerTexture();
}

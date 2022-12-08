#include "Resources/Font.hpp"

Resources::Font::Font()
{
}

Resources::Font::~Font()
{
}

void Resources::Font::Load(const char* path)
{
	ShouldDeleteData = false;
	Texture::Load(path);
	GenerateSpacings();
	Texture::DeleteData();
}

void Resources::Font::UnLoad()
{
	Texture::UnLoad();
	this->~Font();
}

void Resources::Font::GenerateSpacings()
{
	if (!loaded) return;
	Core::Maths::IVec2 TileSize = Core::Maths::IVec2(sizeX / 16, sizeY / 16);
	for (int i = 0; i < 256; i++)
	{
		Core::Maths::IVec2 Pos = Core::Maths::IVec2(i % 16, i / 16) * TileSize;
		char c = TileSize.x;
		while (c > 0)
		{
			bool clean = true;
			for (int j = TileSize.y - 1; j >= 0; j--)
			{
				if (ReadPixel(Pos + Core::Maths::IVec2(c-1, j)).a > 127u)
				{
					clean = false;
					break;
				}
			}
			if (!clean) break;
			c--;
		}
		if (c == 0) c = TileSize.x * 2 / 3;
		FontSpacings[i] = c * 1.0f / TileSize.x;
	}
}
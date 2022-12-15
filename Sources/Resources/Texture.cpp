#include "Resources/Texture.hpp"
#include "Resources/TextureManager.hpp"
#include "Core/Debug/Log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <STB_Image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB_Image/stb_image_write.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const int WrapTable[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };
static const int FilterTable[] = { GL_NEAREST, GL_LINEAR };

using namespace Resources;

unsigned int Texture::filterS = GL_NEAREST;
unsigned int Texture::wrapS = GL_REPEAT;
Texture::Texture()
{
	filter = TextureFilterType::Linear;
	wrap = TextureWrapType::Repeat;
	loaded.Store(false);
}

Texture::~Texture()
{
}

Core::Maths::Color4 Resources::Texture::ReadPixel(Core::Maths::IVec2 pos)
{
	if (!loaded.Load() || !ImageData || pos.x >= sizeX || pos.y >= sizeY)
		return Core::Maths::Color4();
	size_t index = ((size_t)pos.x + ((size_t)sizeX - pos.y - 1) * sizeX) * 4;
	return Core::Maths::Color4(ImageData[index], ImageData[index + 1], ImageData[index + 2], ImageData[index + 3]);
}

void Texture::SetFilterType(unsigned int in)
{
	filterS = in;
}

void Texture::SetWrapType(unsigned int in)
{
	wrapS = in;
}

void Texture::SetFilterType(TextureFilterType in, bool bind)
{
	if (bind) glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetFilterValue(in));
	filter = in;
}

void Texture::SetWrapType(TextureWrapType in, bool bind)
{
	if (bind) glBindTexture(GL_TEXTURE_2D, textureID);
	unsigned int value = GetWrapValue(in);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, value);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, value);
	wrap = in;
}

void Texture::Load(const char* path)
{
	fullPath = path;
	if (loaded.Load()) return;
	int nrChannels;
	int startIndex = 0;
	char tmp;
	for (int i = 0; i < 255; i++)
	{
		tmp = path[i];
		if (tmp == 0)
		{
			break;
		}
		else if (tmp == '\\' || tmp == '/')
		{
			startIndex = i + 1;
		}
	}
	tmp = path[startIndex];
	int index = 0;
	for (int i = startIndex + 1; i < 255 && tmp != 0 && tmp != '.'; i++)
	{
		Name[index] = tmp;
		tmp = path[i];
		index++;
	}
	Name[index] = 0;

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path, &sizeX, &sizeY, &nrChannels, 4);
	if (!data)
	{
		LOGRAW("ERROR   : Could not load file %s: ", path);
		LOG(stbi_failure_reason());
	}
}

void Resources::Texture::EndLoad()
{
	if (loaded.Load())
	{
		UnLoad();
		return;
	}
	if (!data) return;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (ShouldDeleteData)
	{
		stbi_image_free(data);
	}
	else
	{
		ImageData = data;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);

	glBindTextureUnit(textureID, textureID);

	loaded.Store(true);
}

void Resources::Texture::DeleteData()
{
	if (ImageData)
	{
		stbi_image_free(ImageData);
		ImageData = nullptr;
	}
}

unsigned int Resources::Texture::GetTextureID()
{
	return textureID;
}

static int unit = 0;
unsigned int Resources::Texture::BindForRender(TextureFilterType FilterIn, TextureWrapType WrapIn)
{
	unit = (unit + 1) % 16;
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (FilterIn != filter) SetFilterType(FilterIn, false);
	if (WrapIn != wrap) SetWrapType(WrapIn, false);
	return unit;
}

unsigned int Resources::Texture::BindForRender()
{
	return BindForRender(filter, wrap);
}

void Texture::UnLoad()
{
	if (loaded.Load())
	{
		glDeleteTextures(1, &textureID);
	}
	if (ShouldDeleteData) DeleteData();
	this->~Texture();
}

void Texture::Overwrite(const unsigned char* data, unsigned int sizeX, unsigned int sizeY)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	this->sizeX = sizeX;
	this->sizeY = sizeY;
}

const char* Texture::GetPath()
{
	return fullPath.c_str();
}

void Texture::SaveImage(const char* path, unsigned char* data, unsigned int sizeX, unsigned int sizeY)
{
	stbi_flip_vertically_on_write(true);
	std::string name = path;
	name.append("@");
	time_t timeLocal;
	struct tm dateTime;
	char text[64];
	time(&timeLocal);
	localtime_s(&dateTime, &timeLocal);
	strftime(text, 64, "%Y_%m_%d-%H_%M_%S", &dateTime);
	name.append(text);
	name.append(".png");
	if (!stbi_write_png(name.c_str(), sizeX, sizeY, 4, data, sizeX * 4))
	{
		LOGRAW("ERROR   : Could not save file %s: ", path);
		LOG(stbi_failure_reason());
	}
	else
	{
		LOG("Saved screenshot as %s", name.c_str());
	}
}

GLFWimage* Resources::Texture::ReadIcon(const char* path)
{
	int x, y, n;
	unsigned char* data = stbi_load(path, &x, &y, &n, 4);
	if (!data)
	{
		LOGRAW("ERROR   : Could not load file %s: ", path);
		LOG(stbi_failure_reason());
	}
	GLFWimage* iconOut = new GLFWimage();
	iconOut->height = y;
	iconOut->width = x;
	iconOut->pixels = data;
	return iconOut;
}

int Resources::Texture::GetFilterIndex(TextureFilterType type)
{
	return static_cast<int>(type);
}

int Resources::Texture::GetWrapIndex(TextureWrapType type)
{
	return static_cast<int>(type);
}

void Resources::Texture::Save(const char* path, unsigned char* data, unsigned int sizeX, unsigned int sizeY)
{
	stbi_flip_vertically_on_write(true);
	std::string nameOut = path;
	nameOut.append(".png");
	if (!stbi_write_png(nameOut.c_str(), sizeX, sizeY, 4, data, sizeX * 4))
	{
		LOGRAW("ERROR could not save image %s: ", path);
		LOG(stbi_failure_reason());
	}
	else
	{
		LOG("Saved image as %s", nameOut.c_str());
	}
}

bool Resources::Texture::LoadImage(const char* path, TextureData& dataOut)
{
	int n;
	unsigned char* data = stbi_load(path, &dataOut.width, &dataOut.height, &n, 4);
	if (!data)
	{
		LOGRAW("ERROR could not load file %s: ", path);
		LOG(stbi_failure_reason());
		return false;
	}
	dataOut.data = data;
	return true;
}

void Resources::Texture::FreeImage(TextureData& dataOut)
{
	if (!dataOut.data) return;
	stbi_image_free(dataOut.data);
	dataOut.data = nullptr;
}

unsigned int Resources::Texture::GetWrapValue(TextureWrapType type)
{
	return WrapTable[GetWrapIndex(type)];
}

unsigned int Resources::Texture::GetFilterValue(TextureFilterType type)
{
	return FilterTable[GetFilterIndex(type)];
}

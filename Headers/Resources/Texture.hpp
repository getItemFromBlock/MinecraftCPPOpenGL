#pragma once

#include <string>

#include "Core/Maths/Maths.hpp"

#include "Resources/IResource.hpp"
#include "Core/DataStructure/INameable.hpp"

struct GLFWimage;

enum class TextureFilterType : unsigned int
{
	Nearest,
	Linear,
};

enum class TextureWrapType : unsigned int
{
	Repeat,
	MirroredRepeat,
	MirroredClamp,
	ClampToEdge,
	ClampToBorder,
};

namespace Resources
{
	struct TextureData;
	class Texture : public IResource, public Core::DataStructure::INameable
	{
	public:

		Texture();
		~Texture();

		virtual void Load(const char* path) override;
		virtual void EndLoad() override;
		void Overwrite(const unsigned char* data, unsigned int sizeX, unsigned int sizeY);
		const char* GetPath() override;
		virtual void UnLoad() override;

		void DeleteData();

		const char* GetName() override { return Name; }
		unsigned int GetTextureID();
		virtual unsigned int BindForRender(TextureFilterType FilterIn, TextureWrapType WrapIn);
		virtual unsigned int BindForRender();
		int GetTextureWidth() { return sizeX; }
		int GetTextureHeight() { return sizeY; }
		Core::Maths::Color4 ReadPixel(Core::Maths::IVec2 pos);

		static void SetFilterType(unsigned int in);
		static void SetWrapType(unsigned int in);

		void SetFilterType(TextureFilterType in, bool bind = true);
		void SetWrapType(TextureWrapType in, bool bind = true);
		static int GetFilterIndex(TextureFilterType type);
		static int GetWrapIndex(TextureWrapType type);
		static unsigned int GetWrapValue(TextureWrapType type);
		static unsigned int GetFilterValue(TextureFilterType type);

		static void SaveImage(const char* path, unsigned char* data, unsigned int sizeX, unsigned int sizeY);
		static void Save(const char* path, unsigned char* data, unsigned int sizeX, unsigned int sizeY);
		static bool LoadImage(const char* path, TextureData& dataOut);
		static void FreeImage(TextureData& dataOut);
		static GLFWimage* ReadIcon(const char* path);
	protected:
		char Name[256] = "Texture";
		std::string fullPath = "";
		unsigned int textureID = 0;
		int sizeX = 0;
		int sizeY = 0;
		TextureFilterType filter;
		TextureWrapType wrap;
		static unsigned int filterS;
		static unsigned int wrapS;
		bool ShouldDeleteData = true;
		unsigned char* ImageData = nullptr;
		unsigned char* data = nullptr;
	};
}

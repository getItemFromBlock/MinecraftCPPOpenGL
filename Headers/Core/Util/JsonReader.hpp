#pragma once

#include <string>
#include <filesystem>

#include "Shape.hpp"

namespace Core::Util
{
	class JsonReader
	{
	public:
		static bool ReadBlockShape(Resources::TextureAtlas* atlas, std::string blockID, Core::Util::Shape shapeIn[7]);
		static bool LoadFile(const std::filesystem::path& path, std::string& out);
	private:
	};
}
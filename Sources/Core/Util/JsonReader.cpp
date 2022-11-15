#include "Core/Util/JsonReader.hpp"

#include <JSON/json.hpp>

void ReadFaces(nlohmann::json& faces, std::unordered_map<std::string, std::string>* textures, Resources::TextureAtlas* atlas, Core::Util::Shape shapes[7], const Core::Maths::Vec3D& from, const Core::Maths::Vec3D& to);
char StringToSide(const std::string& value);

bool Core::Util::JsonReader::LoadFile(const std::filesystem::path& path, std::string& out)
{
	std::ifstream in;
	in.open(path, std::ios::binary | std::ios::in | std::ios::ate);
	if (!in.is_open())
	{
		return false;
	}
	size_t length = in.tellg();
	in.seekg(0, in.beg);
	out.resize(length + 1);
	
	in.read(out.data(), length);
	in.close();
	out.data()[length] = 0;
	return true;
}

bool Core::Util::JsonReader::ReadBlockShape(Resources::TextureAtlas* atlas, std::string blockID, Core::Util::Shape shapeIn[7])
{
	std::unordered_map<std::string, std::string> textures;
	std::string parent = blockID;
	do
	{
		std::string fullpath = "Resources/models/";
		fullpath.append(parent);
		fullpath.append(".json");
		parent = "";
		if (!std::filesystem::exists(fullpath)) return false;
		std::string content;
		if (!LoadFile(fullpath, content)) return false;
		auto j = nlohmann::json::parse(content);
		if (!j["parent"].is_null())
		{
			parent = j["parent"].get<std::string>();
			size_t index = 0;
			for (int64_t i = parent.length(); i >= 0; i--)
				if (parent.c_str()[i] == ':')
				{
					index = i + 1;
					break;
				}
			parent = parent.substr(index);
		}
		if (!j["textures"].is_null())
		{
			for (auto n = j["textures"].begin(); n != j["textures"].end(); n++)
			{
				std::string t1 = n.key();
				std::string t2 = n.value();
				auto e = textures.find(t1);
				if (e == textures.end())
				{
					if (t2.c_str()[0] == '#')
					{
						t2 = t2.substr(1);
						auto tmp = textures.find(t2);
						if (tmp != textures.end())
							t2 = tmp->second;
					}
					else
					{
						size_t index = 0;
						for (int64_t i = t2.length(); i >= 0; i--)
							if (t2.c_str()[i] == ':')
							{
								index = i + 1;
								break;
							}
						t2 = t2.substr(index);
					}
					textures.insert(std::pair<std::string, std::string>(t1, t2));
				}
			}
		}
		if (!j["elements"].is_null())
		{
			nlohmann::json& elements = j["elements"];
			for (auto n = elements.begin(); n != elements.end(); n++)
			{
				auto y = n->at("from").get<std::vector<int>>();
				Core::Maths::Vec3D from = Core::Maths::Vec3D((float)y[0], (float)y[1], (float)y[2]) / 16.0f;
				y = n->at("to").get<std::vector<int>>();
				Core::Maths::Vec3D to = Core::Maths::Vec3D((float)y[0], (float)y[1], (float)y[2]) / 16.0f;
				nlohmann::json& faces = n->at("faces");
				ReadFaces(faces, &textures, atlas, shapeIn, from, to);
			}
		}
	} while (parent.c_str()[0]);
	return true;
}

void GetDefaultUV(char side, std::vector<int>& values, const Core::Maths::Vec3D& from, const Core::Maths::Vec3D& to)
{
	if (side == 0 || side == 3)
	{
		values = { int(from.z * 16), int(from.y * 16),int(to.z * 16),int(to.y * 16) };

	}
	else if (side == 2 || side == 5)
	{
		values = { int(from.x * 16), int(from.y * 16),int(to.x * 16),int(to.y * 16) };
	}
	else
	{
		values = { int(from.z * 16), int(from.x * 16),int(to.z * 16),int(to.x * 16) };
	}
}

void ReadFaces(nlohmann::json& faces, std::unordered_map<std::string, std::string>* textures, Resources::TextureAtlas* atlas, Core::Util::Shape shapes[7], const Core::Maths::Vec3D& from, const Core::Maths::Vec3D& to)
{
	for (auto f = faces.begin(); f != faces.end(); f++)
	{
		char side = 6;
		const std::string& nm = f.key();
		nlohmann::json& content = f.value();
		if (!content["cullface"].is_null())
		{
			std::string tmp = content["cullface"].get<std::string>();
			side = StringToSide(tmp);
		}
		std::string tex = content["texture"].get<std::string>();
		if (tex.c_str()[0] == '#')
		{
			tex = tex.substr(1);
			auto tmp = textures->find(tex);
			if (tmp != textures->end())
				tex = tmp->second;
		}
		else
		{
			size_t index = 0;
			for (int64_t i = tex.length(); i >= 0; i--)
				if (tex.c_str()[i] == ':')
				{
					index = i + 1;
					break;
				}
			tex = tex.substr(index);
		}
		char dir = StringToSide(f.key());
		auto z = content["uv"];
		std::vector<int> y = std::vector<int>(4llu);
		if (z.is_null())
		{
			GetDefaultUV(side, y, from, to);
		}
		else
		{
			y = z.get<std::vector<int>>();
		}
		Core::Maths::Vec2D min = atlas->GetTexture(tex);
		Core::Maths::Vec2D max = atlas->GetTextureSize();
		Core::Maths::Vec2D uvA = Core::Maths::Vec2D((float)y[0llu + (side == 1)], (float)y[1llu - (side == 1)]) / 16.0f;
		Core::Maths::Vec2D uvB = Core::Maths::Vec2D((float)y[2llu + (side == 1)], (float)y[3llu - (side == 1)]) / 16.0f;
		uvA = min + uvA * max;
		uvB = min + uvB * max;
		Core::Util::Shape::CreateFaceShape(dir, shapes[side], from, to, uvA, uvB);
	}
}

char StringToSide(const std::string& value)
{
	if (value == "east") return 0;
	else if (value == "up") return 1;
	else if (value == "north") return 2;
	else if (value == "west") return 3;
	else if (value == "down") return 4;
	else if (value == "south") return 5;
	else return 6;
}

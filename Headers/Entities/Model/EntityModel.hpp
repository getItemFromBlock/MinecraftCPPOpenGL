#pragma once

#include <unordered_map>

#include "BodyPart.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Resources/Texture.hpp"

namespace World
{
	class World;
}

namespace Entities::Model
{
	class EntityModel
	{
	public:
		EntityModel();
		~EntityModel();

		std::unordered_map<std::string, BodyPart*> parts;

		BodyPart* AddOrReplacePart(const std::string&, BodyPart* part);
		BodyPart* GetPart(const std::string name);
		void DeleteModel();
		void Bake();
		void Render(Core::Maths::Vec3 position, float rotation, World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp);
		void SetTexture(Resources::Texture* tex);
	private:
		Resources::Texture* texture = nullptr;
	};
}
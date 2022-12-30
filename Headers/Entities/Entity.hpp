#pragma once

#include "Core/Maths/Maths.hpp"
#include "Model/EntityModel.hpp"

namespace Resources
{
	class ResourceManager;
	class Texture;
}

namespace Entities
{
	class Entity
	{
	public:
		Core::Maths::Vec3 Position;

		Entity();

		virtual void Update(float deltatime);
		static void SetResourceManager(Resources::ResourceManager* in);

		size_t uuid;
	protected:
		static Resources::ResourceManager* manager;
		Resources::Texture* texture = nullptr;
	};
}
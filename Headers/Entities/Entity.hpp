#pragma once

#include "Core/Maths/Maths.hpp"
#include "Model/EntityModel.hpp"

namespace Entities
{
	class Entity
	{
	public:
		Core::Maths::Vec3 Position;

		Entity();

		virtual void Update(float deltatime);

		size_t uuid;
	protected:
	};
}
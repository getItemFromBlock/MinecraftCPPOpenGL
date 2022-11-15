#pragma once

#include "Core/Maths/Maths.hpp"

namespace Entities
{
	class Entity
	{
	public:
		Core::Maths::Vec3D Position;

		Entity();

		virtual void Update(float deltatime);

	protected:
		size_t uuid;
	};
}
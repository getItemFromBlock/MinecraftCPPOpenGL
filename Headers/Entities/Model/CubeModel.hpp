#pragma once

#include "Core/Maths/Maths.hpp"

using namespace Core::Maths;

namespace Entities::Model
{
	class CubeModel
	{
	public:
		CubeModel() {};
		~CubeModel() {};

		Vec3 offset;
		Vec3 size;
		Vec2 texOffset;
		float extend = 0.0f;
		bool mirrored = false;
	private:

	};
}
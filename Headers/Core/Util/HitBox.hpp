#pragma once

#include <vector>

#include "Core/Maths/Maths.hpp"

namespace Core::Util
{
	enum class CollisionType
	{
		DEFAULT,
		PASS,
		NONE,
	};

	class Box
	{
	public:
		Core::Maths::Vec3D center;
		Core::Maths::Vec3D size;
		Box() {}
		Box(Core::Maths::Vec3D boxCenter, Core::Maths::Vec3D boxSize) : center(boxCenter), size(boxSize) {}
	};

	class HitBox
	{
	public:
		std::vector<Core::Util::Box> HitBoxData;

		static void CreateCubeHitBox(HitBox& in);
	private:
	};
}
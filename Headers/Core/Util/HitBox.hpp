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
		Core::Maths::Vec3 center;
		Core::Maths::Vec3 size;
		Box() {}
		Box(Core::Maths::Vec3 boxCenter, Core::Maths::Vec3 boxSize) : center(boxCenter), size(boxSize) {}

		bool IsInside(Core::Maths::Vec3 position) const;
	};

	class HitBox
	{
	public:
		std::vector<Core::Util::Box> HitBoxData;

		static void CreateCubeHitBox(HitBox& in);
	private:
	};
}
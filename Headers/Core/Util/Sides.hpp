#pragma once

#include "Core/Maths/Maths.hpp"

namespace Core::Util
{
	enum class Side : char
	{
		RIGHT,
		UP,
		FRONT,
		LEFT,
		DOWN,
		BACK,
		NONE,
	};

	namespace Sides
	{
		static inline Side GetOpposite(Side in)
		{
			return static_cast<Side>((static_cast<char>(in) + 3) % 6);
		}

		static inline Side GetNext(Side in)
		{
			return static_cast<Side>((static_cast<char>(in) + 1) % 6);
		}

		static inline Side GetPrevious(Side in)
		{
			return static_cast<Side>((static_cast<char>(in) + 5) % 6);
		}

		static inline Core::Maths::IVec3 GetNeighbor(Side in, Core::Maths::IVec3 blockPos)
		{
			switch (in)
			{
			case Core::Util::Side::RIGHT:
				return blockPos + Core::Maths::IVec3(1, 0, 0);
			case Core::Util::Side::UP:
				return blockPos + Core::Maths::IVec3(0, 1, 0);
			case Core::Util::Side::FRONT:
				return blockPos + Core::Maths::IVec3(0, 0, 1);
			case Core::Util::Side::LEFT:
				return blockPos + Core::Maths::IVec3(-1, 0, 0);
			case Core::Util::Side::DOWN:
				return blockPos + Core::Maths::IVec3(0, -1, 0);
			case Core::Util::Side::BACK:
				return blockPos + Core::Maths::IVec3(0, 0, -1);
			default:
				return blockPos + Core::Maths::IVec3(1, 0, 0);
			}
		}
	}
}
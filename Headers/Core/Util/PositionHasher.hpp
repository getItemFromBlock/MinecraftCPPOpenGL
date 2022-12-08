#pragma once

#include "Core/Maths/Maths.hpp"

namespace Core::Util
{
	class PositionHasher
	{
	public:
		size_t operator()(Core::Maths::IVec3 key) const;
	};
}
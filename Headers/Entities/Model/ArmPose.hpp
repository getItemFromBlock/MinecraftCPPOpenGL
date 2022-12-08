#pragma once

namespace Entities
{
	enum class ArmPose : char
	{
		EMPTY = 0,
		BLOCK,
		ITEM,
		THROW_SPEAR,
		BOW_AND_ARROW,
		CROSSBOW_CHARGE,
		CROSSBOW_HOLD,
		SPYGLASS,
	};
}
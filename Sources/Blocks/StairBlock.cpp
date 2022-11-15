#include "Blocks/StairBlock.hpp"

Blocks::StairBlock::StairBlock(const char* Name_ID)
{
	Assert(Name_ID != nullptr);
	std::string tmp = Name_ID;
	Assert(IsValidString(tmp));
	name = tmp;
	hitBox = Core::Util::HitBox();
	hitBox.HitBoxData.push_back((Core::Util::Box(Core::Maths::Vec3D(0.5f, 0.25f, 0.5f), Core::Maths::Vec3D(1.0f, 0.5f, 1.0f))));
	hitBox.HitBoxData.push_back((Core::Util::Box(Core::Maths::Vec3D(0.75f, 0.75f, 0.5f), Core::Maths::Vec3D(0.5f, 0.5f, 1.0f))));
}

Blocks::StairBlock::~StairBlock()
{
}

bool Blocks::StairBlock::IsSideFull(Core::Util::Side side)
{
	return side == Core::Util::Side::DOWN || side == Core::Util::Side::RIGHT;
}

const Core::Util::HitBox& Blocks::StairBlock::GetHitBox()
{
	return hitBox;
}

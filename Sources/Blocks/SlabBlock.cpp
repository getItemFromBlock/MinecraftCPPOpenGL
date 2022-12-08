#include "Blocks/SlabBlock.hpp"

Blocks::SlabBlock::SlabBlock(const char* Name_ID)
{
	Assert(Name_ID != nullptr);
	std::string tmp = Name_ID;
	Assert(IsValidString(tmp));
	name = tmp;
	hitBox = Core::Util::HitBox();
	hitBox.HitBoxData.push_back((Core::Util::Box(Core::Maths::Vec3(0.5f, 0.25f, 0.5f), Core::Maths::Vec3(1.0f, 0.5f, 1.0f))));
}

Blocks::SlabBlock::~SlabBlock()
{
}

bool Blocks::SlabBlock::IsSideFull(Core::Util::Side side)
{
	return side == Core::Util::Side::DOWN;
}

const Core::Util::HitBox& Blocks::SlabBlock::GetHitBox()
{
	return hitBox;
}

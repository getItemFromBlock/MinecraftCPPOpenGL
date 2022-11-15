#include "Blocks/TorchBlock.hpp"

Blocks::TorchBlock::TorchBlock(const char* Name_ID, Core::Maths::Vec3D light) : LightBlock(Name_ID, light)
{
	hitBox = Core::Util::HitBox();
	hitBox.HitBoxData.push_back((Core::Util::Box(Core::Maths::Vec3D(0.5f, 0.3f, 0.5f), Core::Maths::Vec3D(0.25f, 0.6f, 0.25f))));
}

Blocks::TorchBlock::~TorchBlock()
{
}

bool Blocks::TorchBlock::IsSideFull(Core::Util::Side side)
{
	return false;
}

const Core::Util::HitBox& Blocks::TorchBlock::GetHitBox()
{
	return hitBox;
}

Core::Util::CollisionType Blocks::TorchBlock::GetCollisionType()
{
	return Core::Util::CollisionType::PASS;
}

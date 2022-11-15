#include "Blocks/AirBlock.hpp"

Blocks::AirBlock::AirBlock(const char* Name_ID)
{
	Assert(Name_ID != nullptr);
	std::string tmp = Name_ID;
	Assert(IsValidString(tmp));
	name = tmp;
	breakable = false;
}

Blocks::AirBlock::~AirBlock()
{
}

bool Blocks::AirBlock::IsSideFull(Core::Util::Side side)
{
	return false;
}

Core::Util::ShapeType Blocks::AirBlock::GetShapeType()
{
	return Core::Util::ShapeType::EMPTY;
}

Core::Util::CollisionType Blocks::AirBlock::GetCollisionType()
{
	return Core::Util::CollisionType::NONE;
}

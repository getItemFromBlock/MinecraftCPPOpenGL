#include "Blocks/TransparentBlock.hpp"

Blocks::TransparentBlock::TransparentBlock(const char* Name_ID) : Block(Name_ID)
{
}

Blocks::TransparentBlock::~TransparentBlock()
{
}

bool Blocks::TransparentBlock::IsSideFull(Core::Util::Side side)
{
	return false;
}

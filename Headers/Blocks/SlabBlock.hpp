#pragma once

#include "Block.hpp"

namespace Blocks
{
	class SlabBlock : public Block
	{
	public:
		SlabBlock(const char* Name_ID);
		~SlabBlock();

		virtual bool IsSideFull(Core::Util::Side side) override;
		virtual const Core::Util::HitBox& GetHitBox() override;
	protected:
	};
}
#pragma once

#include "Block.hpp"

namespace Blocks
{
	class StairBlock : public Block
	{
	public:
		StairBlock(const char* Name_ID);
		~StairBlock();

		virtual bool IsSideFull(Core::Util::Side side) override;
		virtual const Core::Util::HitBox& GetHitBox() override;
	protected:
	};
}
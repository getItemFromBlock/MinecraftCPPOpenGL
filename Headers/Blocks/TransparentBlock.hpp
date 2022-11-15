#pragma once

#include "Block.hpp"

namespace Blocks
{
	class TransparentBlock : public Block
	{
	public:
		TransparentBlock(const char* Name_ID);
		~TransparentBlock();

		virtual bool IsSideFull(Core::Util::Side side) override;
	protected:
	};
}
#pragma once

#include "Block.hpp"

namespace Blocks
{
	class AirBlock : public Block
	{
	public:
		AirBlock(const char* Name_ID);
		~AirBlock();

		virtual bool IsSideFull(Core::Util::Side side) override;
		virtual Core::Util::ShapeType GetShapeType() override;
		virtual Core::Util::CollisionType GetCollisionType() override;
	protected:
	};
}
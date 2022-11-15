#pragma once

#include "LightBlock.hpp"

namespace Blocks
{
	class TorchBlock : public LightBlock
	{
	public:
		TorchBlock(const char* Name_ID, Core::Maths::Vec3D light);
		~TorchBlock();

		virtual bool IsSideFull(Core::Util::Side side) override;
		virtual const Core::Util::HitBox& GetHitBox() override;
		virtual Core::Util::CollisionType GetCollisionType() override;
	protected:
	};
}
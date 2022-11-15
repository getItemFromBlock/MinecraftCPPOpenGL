#pragma once

#include "Block.hpp"

namespace Blocks
{
	class LightBlock : public Block
	{
	public:
		LightBlock(const char* Name_ID, Core::Maths::Vec3D light);
		~LightBlock();

		virtual bool IsLightBlock() override;
		virtual Core::Maths::Vec3D GetLightValue() override;
	protected:
		Core::Maths::Vec3D lightColor;
	};
}
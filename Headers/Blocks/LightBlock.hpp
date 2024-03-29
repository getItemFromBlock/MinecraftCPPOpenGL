#pragma once

#include "Block.hpp"

namespace Blocks
{
	class LightBlock : public Block
	{
	public:
		LightBlock(const char* Name_ID, Core::Maths::Vec3 light);
		~LightBlock();

		virtual bool IsLightBlock() override;
		virtual Core::Maths::Vec3 GetLightValue() override;
	protected:
		Core::Maths::Vec3 lightColor;
	};
}
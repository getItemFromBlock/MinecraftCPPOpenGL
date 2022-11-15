#include "Blocks/LightBlock.hpp"

Blocks::LightBlock::LightBlock(const char* Name_ID, Core::Maths::Vec3D light) : Block(Name_ID)
{
	lightColor = light;
}

Blocks::LightBlock::~LightBlock()
{
}

bool Blocks::LightBlock::IsLightBlock()
{
	return true;
}

Core::Maths::Vec3D Blocks::LightBlock::GetLightValue()
{
	return lightColor;
}


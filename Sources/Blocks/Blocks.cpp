#include "Blocks/Blocks.hpp"

#include "Blocks/AirBlock.hpp"
#include "Blocks/SlabBlock.hpp"
#include "Blocks/StairBlock.hpp"
#include "Blocks/LightBlock.hpp"
#include "Blocks/TransparentBlock.hpp"
#include "Blocks/TorchBlock.hpp"

using namespace Blocks;

Block* BlockRegister::GetBlock(BLOCK blockIn)
{
	return blocklist[static_cast<unsigned int>(blockIn)];
}

BLOCK Blocks::BlockRegister::getNext(BLOCK in)
{
	if (in == BLOCK::STONE_STAIRS) return BLOCK::STONE;
	return static_cast<BLOCK>(static_cast<unsigned int>(in) + 1);
}

BLOCK Blocks::BlockRegister::getPrevious(BLOCK in)
{
	if (in == BLOCK::STONE) return BLOCK::STONE_STAIRS;
	return static_cast<BLOCK>(static_cast<unsigned int>(in) - 1);
}

void BlockRegister::RegisterBlocks(Resources::TextureAtlas* atlas)
{
	blocklist =
	{
	(new AirBlock("air")),
	(new Block("stone"))->SetHardness(3.0f),
	(new Block("cobblestone"))->SetHardness(3.5f),
	(new Block("deepslate"))->SetHardness(6.0f),
	(new Block("bedrock"))->SetUnbreakable(),
	(new Block("dirt"))->SetHardness(1.0f),
	(new Block("grass_block"))->SetHardness(1.1f),
	(new Block("oak_planks"))->SetHardness(1.5f),
	(new TransparentBlock("glass"))->SetHardness(1.0f),
	(new LightBlock("glowstone", Core::Maths::Vec3(1.0f, 1.0f, 0.7f)))->SetHardness(1.0f),
	(new LightBlock("sea_lantern", Core::Maths::Vec3(0.7f, 0.7f, 1.0f)))->SetHardness(1.0f),
	(new TorchBlock("torch", Core::Maths::Vec3(0.9f, 0.6f, 0.6f)))->SetHardness(0.5f),
	(new TorchBlock("redstone_torch", Core::Maths::Vec3(0.5f, 0.1f, 0.1f)))->SetHardness(0.5f),
	(new Block("command_block"))->SetUnbreakable(),
	(new Block("lectern"))->SetHardness(1.5f),
	(new SlabBlock("oak_slab"))->SetHardness(1.5f),
	(new SlabBlock("stone_slab"))->SetHardness(3.0f),
	(new StairBlock("oak_stairs"))->SetHardness(1.5f),
	(new StairBlock("stone_stairs"))->SetHardness(3.0f),
	};

	for (size_t i = 0; i < blocklist.size(); i++) blocklist[i]->RegisterModel(atlas);
}

void Blocks::BlockRegister::FreeBlocks()
{
	for (size_t i = 0; i < blocklist.size(); i++) delete blocklist.at(i);
	blocklist.clear();
}

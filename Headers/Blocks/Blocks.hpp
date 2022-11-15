#pragma once

#include <vector>

#include "Block.hpp"
#include "Resources/TextureAtlas.hpp"

enum class BLOCK : unsigned int
{
	AIR,
	STONE,
	COBBLESTONE,
	BEDROCK,
	DIRT,
	GRASS_BLOCK,
	OAK_PLANKS,
	GLASS,
	GLOWSTONE,
	SEA_LANTERN,
	TORCH,
	REDSTONE_TORCH,
	COMMAND_BLOCK,
	LECTERN,
	OAK_SLAB,
	STONE_SLAB,
	OAK_STAIRS,
	STONE_STAIRS,
};

namespace Blocks
{
	class BlockRegister
	{
	public:
		static Block* GetBlock(BLOCK blockIn);

		static BLOCK getNext(BLOCK in);
		static BLOCK getPrevious(BLOCK in);

		static void RegisterBlocks(Resources::TextureAtlas* atlas);

		static void FreeBlocks();
	private:
		static inline std::vector<Block*> blocklist;
	};
}
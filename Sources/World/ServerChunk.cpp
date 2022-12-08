#include "World/ServerChunk.hpp"

#include "Blocks/Blocks.hpp"
#include "World/ServerWorld.hpp"

World::ServerChunk::ServerChunk()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		for (unsigned int j = 0; j < 16; j++)
			content[i * 16 + j] = Blocks::BlockRegister::GetBlock(BLOCK::AIR);
		heightMap[i] = 0;
	}
}

World::ServerChunk::~ServerChunk()
{
}

void World::ServerChunk::Update(ServerWorld* worldIn)
{
	if (!isReady.load()) return;
	// TODO tick logic
}

void World::ServerChunk::SetBlock(ServerWorld* worldIn, Core::Maths::IVec3 pos, Blocks::Block* block, bool update)
{
	uint16_t bInd = GetBlockPosCk(pos);
	content[bInd] = block;
	uint8_t height = bInd >> 8;
	uint8_t index = bInd & 0xff;
	if (height > heightMap[index]) heightMap[index] = height;
	if (update)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				for (int k = -1; k <= 1; k++)
				{
					Core::Maths::IVec3 otherPos = Core::Maths::IVec3(i, j, k) + pos;
					char mDist = (i != 0) + (j != 0) + (k != 0);
					if (mDist <= 1)
					{
						Blocks::Block* bk = worldIn->GetBlockAt(otherPos);
						if (bk) bk->Update(worldIn, bk, otherPos);
					}
				}
			}
		}
	}
}

void World::ServerChunk::SetBlockNoUpdate(Core::Maths::IVec3 pos, Blocks::Block* block)
{
	uint16_t bInd = GetBlockPosCk(pos);
	content[bInd] = block;
	uint8_t height = bInd >> 8;
	uint8_t index = bInd & 0xff;
	if (height > heightMap[index]) heightMap[index] = height;
}

uint16_t World::ServerChunk::GetBlockPosCk(Core::Maths::IVec3 blockPos)
{
	return Core::Maths::Util::imod(blockPos.x, 16) |
		Core::Maths::Util::imod(blockPos.z, 16) << 4 |
		Core::Maths::Util::imod(blockPos.y, 16) << 8;
}

Blocks::Block* World::ServerChunk::GetBlock(Core::Maths::IVec3 pos)
{
	return content[GetBlockPosCk(pos)];
}

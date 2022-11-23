#include "World/ChunkGenerator.hpp"

#include "Blocks/Blocks.hpp"
#include "World/World.hpp"

World::ChunkGenerator::ChunkGenerator()
{
	worldSeed = static_cast<unsigned int>(rand());
	noiseA = siv::BasicPerlinNoise<float>(worldSeed);
}

World::ChunkGenerator::~ChunkGenerator()
{
}

void World::ChunkGenerator::InitThread(World* worldIn)
{
	world = worldIn;
	generatorThread = new std::thread(&ChunkGenerator::ThreadFunc, this);
}

void World::ChunkGenerator::ExitThread()
{
	shouldExit.store(true);
	generatorThread->join();
	delete generatorThread;
}

void World::ChunkGenerator::GenerateChunk(Chunk* ck)
{
	Core::Maths::Int3D worldPos = ck->getWorldPos();
	for (unsigned int j = 0; j < 16; j++)
	{
		for (unsigned int i = 0; i < 16; i++)
		{
			for (unsigned int k = 0; k < 16; k++)
			{
				Core::Maths::Int3D blockPos = Core::Maths::Int3D(i, j, k) + worldPos * 16;
				float height = wFunc(blockPos.x, blockPos.z);
				if (blockPos.y == -64)
				{
					ck->SetBlockNoUpdate(blockPos, Blocks::BlockRegister::GetBlock(BLOCK::BEDROCK));
				}
				else if (blockPos.y < height)
				{
					if (fabsf(hFunc(blockPos.x, blockPos.y, blockPos.z)) > 0.4f) continue;
					if (blockPos.y >= height - 1)
					{
						ck->SetBlockNoUpdate(blockPos, Blocks::BlockRegister::GetBlock(BLOCK::GRASS_BLOCK));
					}
					else if (blockPos.y >= height - 5)
					{
						ck->SetBlockNoUpdate(blockPos, Blocks::BlockRegister::GetBlock(BLOCK::DIRT));
					}
					else if (blockPos.y >= dFunc(blockPos.z, blockPos.x))
					{
						ck->SetBlockNoUpdate(blockPos, Blocks::BlockRegister::GetBlock(BLOCK::STONE));
					}
					else
					{
						ck->SetBlockNoUpdate(blockPos, Blocks::BlockRegister::GetBlock(BLOCK::DEEPSLATE));
					}
				}
			}
		}
	}
}

unsigned int World::ChunkGenerator::GetWorldSeed()
{
	return worldSeed;
}

void World::ChunkGenerator::ThreadFunc()
{
	while (!shouldExit.load())
	{
		if (!inputAccessFlag.load())
		{
			for (size_t i = 0; i < InputQueue.size(); i++)
			{
				treatedChunks.push_back(InputQueue[i]);
			}
			InputQueue.clear();
			inputAccessFlag.store(true);
		}
		size_t size = treatedChunks.size();
		if (size == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else
		{
			for (size_t i = 0; i < size; i++)
			{
				GenerateChunk(treatedChunks[i]);
				treatedChunks[i]->GenerateRender(world);
			}
			treatedChunks.clear();
		}
	}
}

float World::ChunkGenerator::wFunc(int a, int b)
{
	return 68 + 5 * noiseA.noise2D(a / 16.0f, b / 16.0f);
}

float World::ChunkGenerator::dFunc(int a, int b)
{
	return 3 * noiseA.noise2D(static_cast<float>(a), static_cast<float>(b));
}

float World::ChunkGenerator::hFunc(int a, int b, int c)
{
	return noiseA.noise3D(a / 16.0f, b / 16.0f, c / 16.0f);
}
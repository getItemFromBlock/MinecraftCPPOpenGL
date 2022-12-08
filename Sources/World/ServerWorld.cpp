#include "World/ServerWorld.hpp"

#define DAY_LENGTH 15000

World::ServerWorld::ServerWorld(double initTime)
	: deltaITime(initTime)
{
	//generator.InitThread(this);

	GenerateChunk(Core::Maths::IVec3(0, 0, 0));
	GenerateChunk(Core::Maths::IVec3(-1, 0, 0));
	GenerateChunk(Core::Maths::IVec3(0, 0, -1));
	GenerateChunk(Core::Maths::IVec3(-1, 0, -1));
}

World::ServerWorld::~ServerWorld()
{
}

unsigned int World::ServerWorld::GetSeed()
{
	return generator.GetWorldSeed();
}

bool World::ServerWorld::IsPositionLoaded(Core::Maths::IVec3 pos)
{
	Core::Maths::IVec3 chunkPos = GetChunkPos(pos);
	auto result = chunks.find(chunkPos);
	return (result != chunks.end() && result->second);
}

bool World::ServerWorld::SetBlockAt(Core::Maths::IVec3 pos, Blocks::Block* block)
{
	ServerChunk* ck = GetChunk(pos);
	if (!ck) return false;
	Core::Maths::IVec3 chunkPos = GetChunkPos(pos);
	ck->SetBlock(this, pos, block);
	return true;
}

Blocks::Block* World::ServerWorld::GetBlockAt(Core::Maths::IVec3 pos)
{
	ServerChunk* ck = GetChunk(pos);
	if (!ck) return nullptr;
	return ck->GetBlock(pos);
}

int World::ServerWorld::GetTopBlock(int x, int z)
{
	Core::Maths::IVec3 ckPos = GetChunkPos(Core::Maths::IVec3(x, 0, z));
	int posX = Core::Maths::Util::imod(x, 16);
	int posZ = Core::Maths::Util::imod(z, 16);
	for (int i = -4; i < 20; i++)
	{
		ckPos.y = i;
		auto result = chunks.find(ckPos);
		if (result == chunks.end()) return i * 16 - 1;
		uint8_t h = result->second->heightMap[posX + 16 * posZ];
		if (h >= 15) continue;
		return 16 * i + h;
	}
	return 20 * 16 - 1;
}

void World::ServerWorld::UpdateWorld(double systemTime)
{
	worldTime = static_cast<int64_t>((systemTime - deltaITime) * 20.0);
	/*
	Core::Maths::Vec3 tmpPos = player.Position;
	tmpPos.y = 0;
	if (generator.inputAccessFlag.load())
	{
		for (signed char i = -1; i < 2; i++)
		{
			for (signed char j = -1; j < 2; j++)
			{
				Core::Maths::IVec3 chunkPos = GetChunkPos(tmpPos) + Core::Maths::IVec3(i, 0, j);
				auto result = chunks.find(chunkPos);
				if (result == chunks.end())
				{
					for (int i = -4; i < 20; i++)
					{
						ServerChunk* ck = new ServerChunk();
						chunkPos.y = i;
						ck->worldPos = chunkPos;
						//generator.InputQueue.push_back(ck);
						chunks.emplace(chunkPos, ck);
					}
				}
			}
		}
		generator.inputAccessFlag.store(false);
	}
	Core::Maths::IVec3 playerbPos = Core::Maths::IVec3((int)player.Position.x, (int)player.Position.y, (int)player.Position.z);
	if (playerbPos.y < -96)
	{
		player.Velocity.y = 0.0f;
		player.Position.y = generator.wFunc(playerbPos.x, playerbPos.z) + 1.0f;
	}
	for (char i = 0; i < 3; i++) if (player.Position[i] < 0.0f) playerbPos[i] -= 1;
	std::vector<Blocks::Block*> blocks;
	std::vector<Core::Maths::IVec3> bPos;
	for (int i = playerbPos.x - 1; i <= playerbPos.x + 1; i++)
		for (int j = playerbPos.y - 1; j <= playerbPos.y + 3; j++)
			for (int k = playerbPos.z - 1; k <= playerbPos.z + 1; k++)
			{
				Core::Maths::IVec3 blockPos = Core::Maths::IVec3(i, j, k);
				Blocks::Block* bk = GetBlockAt(blockPos);
				if (bk != nullptr)
				{
					blocks.push_back(bk);
					bPos.push_back(blockPos);
				}
			}
	if (blocks.size() != 0) Physics::PhysicsHandler::UpdatePhysics(&blocks, &bPos, &player);
	for (auto chunk = chunks.begin(); chunk != chunks.end(); chunk++)
	{
		if (Core::Maths::Vec3((chunk->second->getWorldPos() * 16) - player.Position).getLength() < 64)
			chunk->second->Update(this);
	}
	*/
}

void World::ServerWorld::Exit()
{
	generator.ExitThread();
	for (auto chunk = chunks.begin(); chunk != chunks.end(); chunk++)
	{
		delete chunk->second;
	}
}

bool World::ServerWorld::IsChunkLoaded(Core::Maths::IVec3 chunkPos)
{
	auto result = chunks.find(chunkPos);
	return (result != chunks.end());
}

Core::Maths::IVec3 World::ServerWorld::GetChunkPos(Core::Maths::IVec3 blockPos)
{
	Core::Maths::IVec3 result;
	for (char i = 0; i < 3; i++)
	{
		result[i] = (blockPos[i] + (blockPos[i] < 0)) / 16 - (blockPos[i] < 0);
	}
	return result;
}

World::ServerChunk* World::ServerWorld::GetChunkAt(Core::Maths::IVec3 chunkPos)
{
	auto result = chunks.find(chunkPos);
	if (result == chunks.end()) return nullptr;
	return result->second;
}

World::ServerChunk* World::ServerWorld::GetChunk(Core::Maths::IVec3 blockPos)
{
	Core::Maths::IVec3 chunkPos = GetChunkPos(blockPos);
	auto result = chunks.find(chunkPos);
	if (result == chunks.end()) return nullptr;
	return result->second;
}

void World::ServerWorld::GenerateChunk(Core::Maths::IVec3 worldPos)
{
	for (int i = -4; i < 20; i++)
	{
		ServerChunk* ck = new ServerChunk();
		worldPos.y = i;
		ck->worldPos = worldPos;
		//generator.GenerateChunk(ck);
		chunks.emplace(worldPos, ck);
	}
}

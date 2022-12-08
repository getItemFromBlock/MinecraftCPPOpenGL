#pragma once

#include <vector>
#include <unordered_map>

#include "Core/Util/PositionHasher.hpp"
#include "ServerChunk.hpp"
#include "Entities/PlayerEntity.hpp"
#include "ChunkGenerator.hpp"

namespace World
{
	class ServerWorld
	{
	public:
		ServerWorld(double initTime);
		~ServerWorld();
		std::vector<Entities::PlayerEntity> players;
		unsigned int GetSeed();
		bool IsPositionLoaded(Core::Maths::IVec3 pos);
		bool SetBlockAt(Core::Maths::IVec3 pos, Blocks::Block* block);
		Blocks::Block* GetBlockAt(Core::Maths::IVec3 pos);
		int GetTopBlock(int x, int z);
		void UpdateWorld(double systemTime);
		void Exit();
	private:
		std::unordered_map<Core::Maths::IVec3, ServerChunk*, Core::Util::PositionHasher> chunks;
		int64_t worldTime = 0ll;
		double deltaITime = 0;
		ChunkGenerator generator;

		bool IsChunkLoaded(Core::Maths::IVec3 chunkPos);
		Core::Maths::IVec3 GetChunkPos(Core::Maths::IVec3 blockPos);
		ServerChunk* GetChunk(Core::Maths::IVec3 blockPos);
		ServerChunk* GetChunkAt(Core::Maths::IVec3 chunkPos);
		void GenerateChunk(Core::Maths::IVec3 worldPos);
	};
}
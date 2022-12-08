#pragma once

#include <atomic>

#include "Core/Maths/Maths.hpp"
#include "Blocks/Block.hpp"

namespace World
{
	class ServerWorld;
	class ChunkGenerator;

	class ServerChunk
	{
		friend class ServerWorld;
		friend class ChunkGenerator;
	public:
		ServerChunk();
		~ServerChunk();
		void Update(ServerWorld* worldIn);
		void SetBlock(ServerWorld* worldIn, Core::Maths::IVec3 pos, Blocks::Block* block, bool update = true);
		void SetBlockNoUpdate(Core::Maths::IVec3 pos, Blocks::Block* block);
		Blocks::Block* GetBlock(Core::Maths::IVec3 pos);
		const Core::Maths::IVec3& getWorldPos() { return worldPos; }
	private:
		std::vector<Core::Util::Vertice> blockRenderData[4096];
		Core::Maths::IVec3 worldPos;
		Blocks::Block* content[4096];
		uint8_t heightMap[16 * 16];
		std::atomic<bool> isReady = false;
		static uint16_t GetBlockPosCk(Core::Maths::IVec3 blockPos);
	};
}
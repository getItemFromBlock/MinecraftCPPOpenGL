#pragma once

#include <atomic>

#include "Core/Maths/Maths.hpp"
#include "Blocks/Block.hpp"
#include "World/ChunkModel.hpp"

#define PLIGHT_SIZE 8

namespace World
{
	class World;
	class ChunkGenerator;

	struct LightBlockData
	{
		Core::Maths::IVec3 globalPos;
		float distance = 0;
		Blocks::Block* ptr = nullptr;
	};

	class Chunk
	{
		friend class World;
		friend class ChunkGenerator;
	public:
		Chunk();
		~Chunk();
		void Update(World* worldIn);
		void SetDirty() { IsDirty = true; }
		void SetBlock(World* worldIn, Core::Maths::IVec3 pos, Blocks::Block* block, bool update = true);
		void SetBlockNoUpdate(Core::Maths::IVec3 pos, Blocks::Block* block);
		Blocks::Block* GetBlock(Core::Maths::IVec3 pos);
		const Core::Maths::IVec3& getWorldPos() { return worldPos; }
		void AddLightBlock(World* worldIn, Core::Maths::IVec3 pos);
		void RemoveLightBlock(Core::Maths::IVec3 pos);
		void SetupLightForRender(Resources::ShaderProgram* shaderProgram);
	private:
		std::vector<Core::Util::Vertice> blockRenderData[4096];
		Core::Maths::IVec3 worldPos;
		ChunkModel model = ChunkModel();
		Blocks::Block* content[4096];
		uint8_t heightMap[16*16];
		bool IsDirty = false;
		std::atomic<bool> isReady = false;
		uint8_t lightCount = 0;
		LightBlockData lightBlocks[PLIGHT_SIZE];
		static uint16_t GetBlockPosCk(Core::Maths::IVec3 blockPos);
		void UpdateBlockRender(World* worldIn, uint16_t index);
		void UpdateRender(World* worldIn);
		void GenerateRender(World* worldIn);
		void Render(Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp, bool IsShadowMap);
	};
}
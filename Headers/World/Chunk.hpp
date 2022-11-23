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
		Core::Maths::Int3D globalPos;
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
		void SetBlock(World* worldIn, Core::Maths::Int3D pos, Blocks::Block* block, bool update = true);
		void SetBlockNoUpdate(Core::Maths::Int3D pos, Blocks::Block* block);
		Blocks::Block* GetBlock(Core::Maths::Int3D pos);
		const Core::Maths::Int3D& getWorldPos() { return worldPos; }
		void AddLightBlock(World* worldIn, Core::Maths::Int3D pos);
		void RemoveLightBlock(Core::Maths::Int3D pos);
	private:
		std::vector<Core::Util::Vertice> blockRenderData[4096];
		Core::Maths::Int3D worldPos;
		ChunkModel model = ChunkModel();
		Blocks::Block* content[4096];
		uint8_t heightMap[16*16];
		bool IsDirty = false;
		std::atomic<bool> isReady = false;
		uint8_t lightCount = 0;
		LightBlockData lightBlocks[PLIGHT_SIZE];
		static uint16_t GetBlockPosCk(Core::Maths::Int3D blockPos);
		void UpdateBlockRender(World* worldIn, uint16_t index);
		void UpdateRender(World* worldIn);
		void GenerateRender(World* worldIn);
		void Render(Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4D& vp, bool IsShadowMap);
	};
}
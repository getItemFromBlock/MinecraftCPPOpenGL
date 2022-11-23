#pragma once

#include <vector>
#include <unordered_map>

#include "Chunk.hpp"
#include "Entities/PlayerEntity.hpp"
#include "LowRenderer/Lightning/ShadowMapBuffer.hpp"
#include "LowRenderer/Rendering/Camera.hpp"
#include "ChunkGenerator.hpp"

namespace World
{
	class Hasher
	{
	public:
		size_t operator()(Core::Maths::Int3D key) const
		{
			size_t test = (size_t)(static_cast<uint32_t>(key.y) & 0xffff) |
				((size_t)(static_cast<uint32_t>(key.x) & 0xffffff) << 16) |
				((size_t)(static_cast<uint32_t>(key.z) & 0xffffff) << 40);
			return test;
		}
	};
	class World
	{
	public:
		World(double initTime, Resources::MeshManager* meshes, Resources::ShaderProgram* mainShader, Resources::ShaderProgram* litShader, int atlas);
		~World();
		Entities::PlayerEntity player;
		unsigned int GetSeed();
		bool IsPositionLoaded(Core::Maths::Int3D pos);
		bool SetBlockAt(Core::Maths::Int3D pos, Blocks::Block* block);
		Blocks::Block* GetBlockAt(Core::Maths::Int3D pos);
		int GetTopBlock(int x, int z);
		void AddToAsyncUpdate(Chunk* in);
		void UpdateWorld(double systemTime);
		void RenderWorld(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgram, const Core::Maths::Mat4D& vp);
		void UpdateBlockRender(Core::Maths::Int3D pos);
		void Exit();
		LowRenderer::Lightning::ShadowMapBuffer shadowMap; // TODO move
	private:
		std::unordered_map<Core::Maths::Int3D, Chunk*, Hasher> chunks;
		int64_t worldTime = 0ll;
		double deltaITime = 0;
		LowRenderer::Model SunModel;
		Resources::ShaderProgram* MainShader;
		Resources::ShaderProgram* LitShader;
		LowRenderer::Rendering::Camera RenderCam;
		int TextureAtlas = 0;
		std::vector<Chunk*> UpdateList;
		ChunkGenerator generator;

		bool IsChunkLoaded(Core::Maths::Int3D chunkPos);
		Core::Maths::Int3D GetChunkPos(Core::Maths::Int3D blockPos);
		Chunk* GetChunk(Core::Maths::Int3D blockPos);
		Chunk* GetChunkAt(Core::Maths::Int3D chunkPos);
		void GenerateChunk(Core::Maths::Int3D worldPos);
	};
}
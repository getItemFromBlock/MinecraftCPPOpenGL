#pragma once

#include <vector>
#include <unordered_map>

#include "Core/Util/PositionHasher.hpp"
#include "Chunk.hpp"
#include "Entities/ClientPlayerEntity.hpp"
#include "LowRenderer/Lightning/ShadowMapBuffer.hpp"
#include "LowRenderer/Rendering/Camera.hpp"
#include "ChunkGenerator.hpp"
#include "Chat/ChatHandler.hpp"

namespace Physics
{
	class PhysicsHandler;
}

namespace World
{
	class World
	{
		friend Physics::PhysicsHandler;
	public:
		World(double initTime, Resources::ResourceManager* manager, Resources::MeshManager* meshes, Resources::ShaderProgram* mainShader, Resources::ShaderProgram* litShader, Resources::ShaderProgram* textShader, int atlas);
		~World();
		Entities::ClientPlayerEntity* player;
		unsigned int GetSeed();
		bool IsPositionLoaded(Core::Maths::IVec3 pos);
		bool SetBlockAt(Core::Maths::IVec3 pos, Blocks::Block* block);
		Blocks::Block* GetBlockAt(Core::Maths::IVec3 pos);
		Chunk* GetChunk(Core::Maths::IVec3 blockPos);
		int GetTopBlock(int x, int z);
		void AddToAsyncUpdate(Chunk* in);
		void UpdateWorld(double systemTime, float deltaTime);
		void RenderWorld(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgram, const Core::Maths::Mat4& vp);
		void UpdateBlockRender(Core::Maths::IVec3 pos);
		void Exit();
		LowRenderer::Lightning::ShadowMapBuffer shadowMap; // TODO move
	private:
		std::unordered_map<Core::Maths::IVec3, Chunk*, Core::Util::PositionHasher> chunks;
		int64_t worldTime = 0ll;
		double deltaITime = 0;
		LowRenderer::Model SunModel;
		Resources::ShaderProgram* MainShader;
		Resources::ShaderProgram* LitShader;
		Resources::ShaderProgram* TextShader;
		LowRenderer::Rendering::Camera RenderCam;
		int TextureAtlas = 0;
		std::vector<Chunk*> UpdateList;
		ChunkGenerator generator;
		Chat::ChatHandler chat;

		std::unordered_map<size_t, Entities::EntityLivingBase*> entities;
		std::unordered_map<size_t, Entities::PlayerEntity*> players;

		bool IsChunkLoaded(Core::Maths::IVec3 chunkPos);
		Core::Maths::IVec3 GetChunkPos(Core::Maths::IVec3 blockPos);
		Chunk* GetChunkAt(Core::Maths::IVec3 chunkPos);
		void GenerateChunk(Core::Maths::IVec3 worldPos);
	};
}
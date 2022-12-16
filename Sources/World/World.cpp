#include "World/World.hpp"

#include "Blocks/Blocks.hpp"
#include "Physics/PhysicsHandler.hpp"
#include "Core/Debug/Gizmo.hpp"
#include "Core/App/App.hpp"

#include "Entities/PigEntity.hpp"

#define DAY_LENGTH 15000

World::World::World(double initTime, Resources::MeshManager* meshes, Resources::ShaderProgram* mainShader, Resources::ShaderProgram* litShader, int atlas)
	: deltaITime(initTime)
{
	SunModel.CreateFrom(meshes->GetModels("DebugPlane").at(0)->model);
	SunModel.shaderProgram = litShader;
	MainShader = mainShader;
	LitShader = litShader;
	TextureAtlas = atlas;
	shadowMap.SetShadowMapResolution(2048);
	shadowMap.Load("MainShadowMap");
	shadowMap.RefreshSize();
	RenderCam = LowRenderer::Rendering::Camera();
	RenderCam.nearPlane = 1.0f;
	RenderCam.farPlane = 300.0f;
	RenderCam.fov = 40.0f;

	generator.InitThread(this);

	GenerateChunk(Core::Maths::IVec3(0, 0, 0));
	GenerateChunk(Core::Maths::IVec3(-1, 0, 0));
	GenerateChunk(Core::Maths::IVec3(0, 0, -1));
	GenerateChunk(Core::Maths::IVec3(-1, 0, -1));
	player = new Entities::ClientPlayerEntity("getItemFromBlock");
	player->Position = Core::Maths::Vec3(0.5f, static_cast<int>(generator.wFunc(0, 0)) + 1.0f, 0.5f);
	Entities::PlayerEntity* player2 = new Entities::PlayerEntity();
	player2->Position = player->Position;
	entities[player->uuid] = player;
	entities[player2->uuid] = player2;
	players[player->uuid] = player;
	players[player2->uuid] = player2;
	Entities::PigEntity* pig = new Entities::PigEntity();
	pig->Position = Core::Maths::Vec3(2.5f, static_cast<int>(generator.wFunc(2, 0)) + 1.0f, 0.5f);
	entities[pig->uuid] = pig;
}

World::World::~World()
{
}

unsigned int World::World::GetSeed()
{
	return generator.GetWorldSeed();
}

bool World::World::IsPositionLoaded(Core::Maths::IVec3 pos)
{
	Core::Maths::IVec3 chunkPos = GetChunkPos(pos);
	auto result = chunks.find(chunkPos);
	return (result != chunks.end() && result->second);
}

bool World::World::SetBlockAt(Core::Maths::IVec3 pos, Blocks::Block* block)
{
	Chunk* ck = GetChunk(pos);
	if (!ck) return false;
	Core::Maths::IVec3 chunkPos = GetChunkPos(pos);
	ck->SetBlock(this, pos, block);
	for (signed char i = -1; i < 2; i++)
	{
		for (signed char j = -1; j < 2; j++)
		{
			for (signed char k = -1; k < 2; k++)
			{
				Chunk* local = GetChunkAt(chunkPos + Core::Maths::IVec3(i, j, k));
				if (local)
				{
					if (block->IsLightBlock())
					{
						local->AddLightBlock(this, pos);
					}
					else
					{
						local->RemoveLightBlock(pos);
					}
				}
			}
		}
	}
	return true;
}

Blocks::Block* World::World::GetBlockAt(Core::Maths::IVec3 pos)
{
	Chunk* ck = GetChunk(pos);
	if (!ck) return nullptr;
	return ck->GetBlock(pos);
}

int World::World::GetTopBlock(int x, int z)
{
	Core::Maths::IVec3 ckPos = GetChunkPos(Core::Maths::IVec3(x,0,z));
	int posX = Core::Maths::Util::imod(x, 16);
	int posZ = Core::Maths::Util::imod(z, 16);
	for (int i = -4; i < 20; i++)
	{
		ckPos.y = i;
		auto result = chunks.find(ckPos);
		if (result == chunks.end()) return i * 16 - 1;
		uint8_t h = result->second->heightMap[posX + 16*posZ];
		if (h >= 15) continue;
		return 16 * i + h;
	}
	return 20*16-1;
}

void World::World::AddToAsyncUpdate(Chunk* in)
{
	UpdateList.push_back(in);
}

void World::World::UpdateWorld(double systemTime, float deltaTime)
{
	worldTime = static_cast<int64_t>((systemTime - deltaITime) * 20.0);
	for (auto e = entities.begin(); e != entities.end(); e++)
	{
		Entities::EntityLivingBase* entity = e->second;
		e->second->Update(deltaTime);
		Core::Maths::IVec3 entityPos = IVec3((int)roundf(entity->Position.x - 0.5f), (int)roundf(entity->Position.y - 0.5f), (int)roundf(entity->Position.z - 0.5f));
		if (entityPos.y < -96)
		{
			entity->Velocity.y = 0.0f;
			entity->Position.y = generator.wFunc(entityPos.x, entityPos.z) + 1.0f;
		}
		std::vector<Blocks::Block*> blocks;
		std::vector<Core::Maths::IVec3> bPos;
		for (int i = entityPos.x - 1; i <= entityPos.x + 1; i++)
			for (int j = entityPos.y - 1; j <= entityPos.y + 3; j++)
				for (int k = entityPos.z - 1; k <= entityPos.z + 1; k++)
				{
					Core::Maths::IVec3 blockPos = Core::Maths::IVec3(i, j, k);
					Blocks::Block* bk = GetBlockAt(blockPos);
					if (bk != nullptr)
					{
						blocks.push_back(bk);
						bPos.push_back(blockPos);
					}
				}
		if (blocks.size() != 0) Physics::PhysicsHandler::UpdatePhysics(&blocks, &bPos, entity);
		Physics::PhysicsHandler::UpdateEntityPhysics(entities, e, deltaTime);
	}
	Core::Maths::Vec3 tmpPos = player->Position;
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
						Chunk* ck = new Chunk();
						chunkPos.y = i;
						ck->worldPos = chunkPos;
						generator.InputQueue.push_back(ck);
						chunks.emplace(chunkPos, ck);
					}
				}
			}
		}
		generator.inputAccessFlag.store(false);
	}
	for (auto chunk = chunks.begin(); chunk != chunks.end(); chunk++)
	{
		if (Core::Maths::Vec3((chunk->second->getWorldPos()*16) - player->Position).getLength() < 64)
			chunk->second->Update(this);
	}
	if (UpdateList.size() > 0)
	{
		UpdateList.back()->UpdateRender(this);
		UpdateList.pop_back();
	}
	player->ClientUpdate(deltaTime, this);
	for (auto e = entities.begin(); e != entities.end(); e++)
	{
		e->second->SetupAnim(deltaTime, static_cast<float>(systemTime));
	}
}

void World::World::RenderWorld(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgram, const Core::Maths::Mat4& vp)
{
	float SunAngle = 360.0f * ((worldTime % DAY_LENGTH) / (float)DAY_LENGTH + 0.1f);
	Core::Maths::Vec3 SunDir = (Core::Maths::Mat4::CreateZRotationMatrix(SunAngle) * Core::Maths::Vec4(-1.0f, 0.0f, 0.0f)).getVector();
	Core::Maths::Mat4 SunMat = Core::Maths::Mat4::CreateTranslationMatrix(player->Position) *
		Core::Maths::Mat4::CreateZRotationMatrix(SunAngle) *
		Core::Maths::Mat4::CreateTranslationMatrix(Core::Maths::Vec3(100.0f, 0.0f, 0.0f)) *
		Core::Maths::Mat4::CreateYRotationMatrix(270.0f) *
		Core::Maths::Mat4::CreateScaleMatrix(10.0f);
	Core::Maths::Mat4 MoonMat = Core::Maths::Mat4::CreateTranslationMatrix(player->Position) *
		Core::Maths::Mat4::CreateZRotationMatrix(180.0f + SunAngle) *
		Core::Maths::Mat4::CreateTranslationMatrix(Core::Maths::Vec3(100.0f, 0.0f, 0.0f)) *
		Core::Maths::Mat4::CreateYRotationMatrix(270.0f) *
		Core::Maths::Mat4::CreateScaleMatrix(7.0f);
	RenderCam.Update(Core::Maths::IVec2(shadowMap.GetTextureWidth(),shadowMap.GetTextureHeight()), (SunDir.y <= 0.0f ? SunMat.GetPositionFromTranslation() : MoonMat.GetPositionFromTranslation()), player->Position);
	Core::Maths::Mat4 shadowMatrix = RenderCam.GetOrthoMatrix() * RenderCam.GetViewMatrix();
	glUseProgram(LitShader->GetProgramID());
	*shaderProgram = LitShader;
	glUniform1i(LitShader->GetLocation(Resources::ShaderData::Texture), TextureAtlas);
	//glCullFace(GL_FRONT);
	shadowMap.BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	for (auto chunk = chunks.begin(); chunk != chunks.end(); chunk++)
	{
		if (Core::Maths::Vec3((chunk->second->getWorldPos() * 16) - player->Position).getLength() < 64)
			chunk->second->Render(*shaderProgram, VAOCurrent, shadowMatrix, true);
	}
	for (auto e = entities.begin(); e != entities.end(); e++)
	{
		e->second->Render(this, *shaderProgram, VAOCurrent, shadowMatrix, true);
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	shadowMap.BindForReading();
	Core::Maths::IVec2 res = Core::App::App::GetMainCamera()->GetResolution();
	glViewport(0, 0, res.x, res.y);
	//glCullFace(GL_BACK);
	float deltaD = SunDir.y < -0.2f ? 1.0f : (SunDir.y > 0.1f ? 0.0f : (-10*(SunDir.y-0.1f)/3));
	float deltaN = SunDir.y > 0.2f ? 1.0f : (SunDir.y < -0.1f ? 0.0f : (10*(SunDir.y+0.1f)/3));
	if (deltaN > deltaD) SunDir = -SunDir;
	float dData[13] = {0.3f * deltaD + 0.139f * deltaN, 0.3f * deltaD + 0.162f * deltaN, 0.3f * deltaD + 0.289f * deltaN,
		0.8f * deltaD + 0.37f * deltaN, 0.8f * deltaD + 0.433f * deltaN, 0.8f * deltaD + 0.778f * deltaN,
		deltaD + deltaN, deltaD + deltaN, deltaD + deltaN,
		2.0f, SunDir.x, SunDir.y, SunDir.z};
	glUseProgram(MainShader->GetProgramID());
	*shaderProgram = MainShader;
	glUniform3f(MainShader->GetLocation(Resources::ShaderData::MatEmissive), 0.0f, 0.0f, 0.0f);
	glUniform3f(MainShader->GetLocation(Resources::ShaderData::MatAmbient), 1.0f, 1.0f, 1.0f);
	glUniform3f(MainShader->GetLocation(Resources::ShaderData::MatDiffuse), 0.6f, 0.6f, 0.6f);
	glUniform3f(MainShader->GetLocation(Resources::ShaderData::MatSpecular), 0.4f, 0.4f, 0.4f);
	glUniform1f(MainShader->GetLocation(Resources::ShaderData::MatSmoothness), 1.0f);
	glUniform1f(MainShader->GetLocation(Resources::ShaderData::MatAbsorbtion), 1.0f);
	glUniform1f(MainShader->GetLocation(Resources::ShaderData::MatShininess), 1.0f);
	glUniform1f(MainShader->GetLocation(Resources::ShaderData::MatAlpha), 1.0f);
	glUniform1i(MainShader->GetLocation(Resources::ShaderData::Texture), TextureAtlas);
	glUniform1ui(MainShader->GetLocation(Resources::ShaderData::LPointCount), 0);
	glUniform1fv(MainShader->GetLocation(Resources::ShaderData::LDirectional), 13, dData);
	glUniform1i(MainShader->GetLocation(Resources::ShaderData::LShadowMap), shadowMap.GetTextureID());
	glUniformMatrix4fv(MainShader->GetLocation(Resources::ShaderData::LightMVP), 1, GL_FALSE, shadowMatrix.content);
	Core::Maths::Vec3 pos = Core::App::App::GetMainCamera()->position;
	glUniform3f(MainShader->GetLocation(Resources::ShaderData::CameraPosition), pos.x, pos.y, pos.z);
	for (auto chunk = chunks.begin(); chunk != chunks.end(); chunk++)
	{
		if (Core::Maths::Vec3((chunk->second->getWorldPos() * 16) - player->Position).getLength() < 64)
		{
			chunk->second->Render(*shaderProgram, VAOCurrent, vp, false);
			if (Core::App::App::IsDebugMode())
			{
				Core::Debug::Gizmo::PushElement(Core::Util::Box(Core::Maths::Vec3(8) + chunk->second->getWorldPos() * 16, Core::Maths::Vec3(16)), Core::Maths::Vec3(1, 1, 0), true);
			}
		}
	}
	for (auto e = entities.begin(); e != entities.end(); e++)
	{
		e->second->Render(this, *shaderProgram, VAOCurrent, vp, false);
	}
	glUniform1ui(MainShader->GetLocation(Resources::ShaderData::LPointCount), 0);
	SunModel.modelMat = SunMat;
	SunModel.SetColor(Core::Maths::Vec3(1.0f));
	SunModel.Render(VAOCurrent, shaderProgram, vp, nullptr);
	SunModel.modelMat = MoonMat;
	SunModel.SetColor(Core::Maths::Vec3(0.8f, 0.8f, 1.0f));
	SunModel.Render(VAOCurrent, shaderProgram, vp, nullptr);
}

void World::World::UpdateBlockRender(Core::Maths::IVec3 pos)
{
	Chunk* ck = GetChunk(pos);
	if (ck)
	{
		ck->UpdateBlockRender(this, ck->GetBlockPosCk(pos));
		ck->SetDirty();
	}
}

void World::World::Exit()
{
	generator.ExitThread();
	for (auto chunk = chunks.begin(); chunk != chunks.end(); chunk++)
	{
		delete chunk->second;
	}
	for (auto e = entities.begin(); e != entities.end(); e++)
	{
		delete e->second;
	}
}

bool World::World::IsChunkLoaded(Core::Maths::IVec3 chunkPos)
{
	auto result = chunks.find(chunkPos);
	return (result != chunks.end());
}

Core::Maths::IVec3 World::World::GetChunkPos(Core::Maths::IVec3 blockPos)
{
	Core::Maths::IVec3 result;
	for (char i = 0; i < 3; i++)
	{
		result[i] = (blockPos[i]+ (blockPos[i] < 0)) / 16 - (blockPos[i] < 0);
	}
	return result;
}

World::Chunk* World::World::GetChunkAt(Core::Maths::IVec3 chunkPos)
{
	auto result = chunks.find(chunkPos);
	if (result == chunks.end()) return nullptr;
	return result->second;
}

World::Chunk* World::World::GetChunk(Core::Maths::IVec3 blockPos)
{
	Core::Maths::IVec3 chunkPos = GetChunkPos(blockPos);
	auto result = chunks.find(chunkPos);
	if (result == chunks.end()) return nullptr;
	return result->second;
}

void World::World::GenerateChunk(Core::Maths::IVec3 worldPos)
{
	for (int i = -4; i < 20; i++)
	{
		Chunk* ck = new Chunk();
		worldPos.y = i;
		ck->worldPos = worldPos;
		generator.GenerateChunk(ck);
		chunks.emplace(worldPos, ck);
	}
	for (int i = -4; i < 20; i++)
	{
		worldPos.y = i;
		Chunk* ck = chunks.at(worldPos);
		ck->GenerateRender(this);
	}
}

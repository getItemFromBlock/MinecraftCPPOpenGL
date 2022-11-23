#include "World/Chunk.hpp"

#include "Blocks/Blocks.hpp"
#include "Core/Util/ModelLoader.hpp"
#include "World/World.hpp"

World::Chunk::Chunk()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		for (unsigned int j = 0; j < 16; j++)
			content[i*16+j] = Blocks::BlockRegister::GetBlock(BLOCK::AIR);
		heightMap[i] = 0;
	}
}

World::Chunk::~Chunk()
{
}

void World::Chunk::Update(World* worldIn)
{
	if (!isReady.load()) return;
	if (IsDirty)
	{
		worldIn->AddToAsyncUpdate(this);
		IsDirty = false;
	}
}

void World::Chunk::SetBlock(World* worldIn, Core::Maths::Int3D pos, Blocks::Block* block, bool update)
{
	uint16_t bInd = GetBlockPosCk(pos);
	content[bInd] = block;
	uint8_t height = bInd >> 8;
	uint8_t index = bInd & 0xff;
	if (height > heightMap[index]) heightMap[index] = height;
	if (update)
	{
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				for (int k = -1; k <= 1; k++)
				{
					Core::Maths::Int3D otherPos = Core::Maths::Int3D(i, j, k) + pos;
					char mDist = (i != 0) + (j != 0) + (k != 0);
					if (mDist <= 1)
					{
						Blocks::Block* bk = worldIn->GetBlockAt(otherPos);
						if (bk) bk->Update(worldIn, bk, otherPos);
					}
					worldIn->UpdateBlockRender(otherPos);
				}
	}
}

void World::Chunk::SetBlockNoUpdate(Core::Maths::Int3D pos, Blocks::Block* block)
{
	uint16_t bInd = GetBlockPosCk(pos);
	content[bInd] = block;
	uint8_t height = bInd >> 8;
	uint8_t index = bInd & 0xff;
	if (height > heightMap[index]) heightMap[index] = height;
}

uint16_t World::Chunk::GetBlockPosCk(Core::Maths::Int3D blockPos)
{
	return Core::Maths::Util::imod(blockPos.x, 16) |
		Core::Maths::Util::imod(blockPos.z, 16) << 4 |
		Core::Maths::Util::imod(blockPos.y, 16) << 8;
}

Blocks::Block* World::Chunk::GetBlock(Core::Maths::Int3D pos)
{
	return content[GetBlockPosCk(pos)];
}

void World::Chunk::AddLightBlock(World* worldIn, Core::Maths::Int3D pos)
{
	LightBlockData data = LightBlockData{pos, Core::Maths::Vec3D(pos - worldPos*16).getLength(), worldIn->GetBlockAt(pos)};
	for (uint8_t i = 0; i < PLIGHT_SIZE; i++)
	{
		if (i >= lightCount)
		{
			lightBlocks[i] = data;
			lightCount++;
			return;
		}
		else if (data.distance < lightBlocks[i].distance)
		{
			for (uint8_t n = PLIGHT_SIZE - 1; n > i; n--)
			{
				lightBlocks[n] = lightBlocks[n-1];
			}
			lightBlocks[i] = data;
			if (lightCount < PLIGHT_SIZE) lightCount++;
			return;
		}
	}
}

void World::Chunk::RemoveLightBlock(Core::Maths::Int3D pos)
{
	for (uint8_t i = 0; i < lightCount; i++)
	{
		if (lightBlocks[i].globalPos == pos)
		{
			for (uint8_t n = i; n < PLIGHT_SIZE - 1; n++)
			{
				lightBlocks[n] = lightBlocks[n+1];
			}
			lightCount--;
			return;
		}
	}
}

void World::Chunk::UpdateBlockRender(World* worldIn, uint16_t index)
{
	Blocks::Block* current = content[index];
	blockRenderData[index].clear();
	if (current->GetShapeType() == Core::Util::ShapeType::EMPTY)
	{
		return;
	}
	Core::Maths::Int3D blockPos = Core::Maths::Int3D(index & 0xf, (index & 0xf00) >> 8, (index & 0xf0) >> 4) + worldPos * 16;
	current->GetFaceShape(Core::Util::Side::NONE).AddToChunk(blockPos, &blockRenderData[index]);
	Core::Util::Side side = Core::Util::Side::RIGHT;
	do
	{
		Blocks::Block* neighbour = worldIn->GetBlockAt(Core::Util::Sides::GetNeighbor(side, blockPos));
		if (neighbour == nullptr) neighbour = Blocks::BlockRegister::GetBlock(BLOCK::AIR);
		if (neighbour->GetShapeType() == Core::Util::ShapeType::EMPTY || !neighbour->IsSideFull(Core::Util::Sides::GetOpposite(side)))
		{
			current->GetFaceShape(side).AddToChunk(blockPos, &blockRenderData[index]);
		}
		side = Core::Util::Sides::GetNext(side);
	} while (side != Core::Util::Side::RIGHT && side != Core::Util::Side::NONE);
}

void World::Chunk::UpdateRender(World* worldIn)
{
	std::vector<Core::Util::Vertice> vertices;
	for (unsigned int x = 0; x < 16; x++)
	{
		for (unsigned int z = 0; z < 16; z++)
		{
			for (unsigned int y = 0; y < 16; y++)
			{
				uint16_t index = x + z * 16 + y * 256;
				//if (y > heightMap[x + z * 16]) break;
				size_t pos = vertices.size();
				vertices.resize(pos + blockRenderData[index].size());
				std::copy(blockRenderData[index].data(), blockRenderData[index].data() + blockRenderData[index].size(), vertices.data()+pos);
			}
		}
	}
	Core::Util::ModelLoader::LoadChunk(&model , &vertices);
}

void World::Chunk::GenerateRender(World* worldIn)
{
	for (char x = 0; x < 16; x++)
	{
		for (char z = 0; z < 16; z++)
		{
			for (char y = 0; y < 16; y++)
			{
				uint16_t index = x + (z << 4) + (y << 8);
				blockRenderData[index] = std::vector< Core::Util::Vertice>();
				if (y > heightMap[x + z * 16]) break;
				UpdateBlockRender(worldIn, index);
			}
		}
	}
	IsDirty = true;
	isReady.store(true);
}

void World::Chunk::Render(Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4D& vp, bool IsShadowMap)
{
	if (!model.IsLoaded()) return;
	if (!IsShadowMap)
	{
		Core::Maths::Vec3D lightData[2 * PLIGHT_SIZE];
		for (uint8_t i = 0; i < lightCount; i++)
		{
			lightData[i*2] = Core::Maths::Vec3D(0.5f) + lightBlocks[i].globalPos;
			lightData[i*2 + 1] = lightBlocks[i].ptr->GetLightValue();
		}
		glUniform1fv(shaderProgram->GetLocation(Resources::ShaderData::LPoint), 6*PLIGHT_SIZE, &lightData[0].x);
		glUniform1ui(shaderProgram->GetLocation(Resources::ShaderData::LPointCount), static_cast<uint32_t>(lightCount));
	}
	model.Render(shaderProgram, VAOCurrent, vp);
}

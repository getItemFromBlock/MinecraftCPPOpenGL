#include "Blocks/Block.hpp"

#include "Core/Debug/Assert.hpp"
#include "Core/Util/JsonReader.hpp"
#include "World/ServerWorld.hpp"

Blocks::Block::Block(const char* Name_ID)
{
	Assert(Name_ID != nullptr);
	std::string tmp = Name_ID;
	Assert(IsValidString(tmp));
	name = tmp;
	Core::Util::HitBox::CreateCubeHitBox(hitBox);
}

Blocks::Block::~Block()
{
}

const Core::Util::Shape& Blocks::Block::GetFaceShape(Core::Util::Side side)
{
	return shapes[static_cast<int>(side)];
}

Core::Util::ShapeType Blocks::Block::GetShapeType()
{
    return Core::Util::ShapeType::DEFAULT;
}

const Core::Util::HitBox& Blocks::Block::GetHitBox()
{
	return hitBox;
}

Core::Util::CollisionType Blocks::Block::GetCollisionType()
{
	return Core::Util::CollisionType::DEFAULT;
}

void Blocks::Block::Update(World::ServerWorld* worldIn, Blocks::Block* other, Core::Maths::IVec3 otherPos)
{
}

Blocks::Block* Blocks::Block::SetHardness(float value)
{
	hardness = value;
	return this;
}

Blocks::Block* Blocks::Block::SetUnbreakable()
{
	breakable = false;
	return this;
}

Blocks::Block::Block()
{
}

bool Blocks::Block::IsValidString(const std::string& in)
{
	if (in.length() < 1 || in.length() > 100) return false;
	for (int i = 0; i < in.length(); i++)
	{
		char n = in.at(i);
		if (n != '_' && (n < 'a' || n > 'z')) return false;
	}
	return true;
}

void Blocks::Block::RegisterModel(Resources::TextureAtlas* atlas)
{
	if (!Core::Util::JsonReader::ReadBlockShape(atlas, std::string("block/").append(name), shapes))
	{
		LOG("Warning! Could not read block model : %s",name.c_str());
	}
}

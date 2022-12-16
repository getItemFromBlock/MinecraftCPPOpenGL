#include "Entities/Model/EntityModel.hpp"

#include <glad/glad.h>

#include "World/World.hpp"

using namespace Entities::Model;

EntityModel::EntityModel()
{
}

EntityModel::~EntityModel()
{
}

BodyPart* EntityModel::AddOrReplacePart(const std::string& name, BodyPart* part)
{
	parts.insert_or_assign(name, part);
	return part;
}

BodyPart* Entities::Model::EntityModel::GetPart(const std::string name)
{
	return nullptr;
}

void Entities::Model::EntityModel::DeleteModel()
{
	for (auto i = parts.begin(); i != parts.end(); i++)
	{
		delete i->second;
	}
	parts.clear();
}

void EntityModel::Bake()
{
	Vec2 texSize = texture ? Vec2((float)texture->GetTextureWidth(), (float)texture->GetTextureHeight()) : Vec2(64, 64);
	for (auto i = parts.begin(); i != parts.end(); i++)
	{
		i->second->Bake(texSize);
	}
}

void EntityModel::Render(Core::Maths::Vec3 position, float rotation, World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp)
{
	if (!shaderProgram || parts.size() == 0) return;
	Core::Maths::Mat4 parentMat = Mat4::CreateTransformMatrix(position, Vec3(0, rotation + 180.0f, 0));
	for (auto i = parts.begin(); i != parts.end(); i++)
	{
		Vec3 offset = Vec3(i->second->offset.x, i->second->offset.y, -i->second->offset.z);
		Mat4 modelMat = parentMat * Mat4::CreateTransformMatrix(offset / -16.0f, Vec3(-Util::toDegrees(i->second->rotation.x), -Util::toDegrees(i->second->rotation.y), Util::toDegrees(i->second->rotation.z)), Vec3(1) / 16.0f);
		Mat4 mvp = vp * modelMat;
		World::Chunk* currentCk = worldIn->GetChunk(position + Core::Maths::Vec3(0.5f));
		if (currentCk) currentCk->SetupLightForRender(shaderProgram);
		glUniformMatrix4fv(shaderProgram->GetLocation(Resources::ShaderData::MVP), 1, GL_FALSE, mvp.content);
		glUniformMatrix4fv(shaderProgram->GetLocation(Resources::ShaderData::M), 1, GL_FALSE, modelMat.content);
		if (texture)
		{
			glUniform1i(shaderProgram->GetLocation(Resources::ShaderData::Texture), texture->GetTextureID());
		}
		else
		{
			glUniform1i(shaderProgram->GetLocation(Resources::ShaderData::Texture), 1);
		}
		i->second->Render(VAOCurrent);
	}
}

void Entities::Model::EntityModel::SetTexture(Resources::Texture* tex)
{
	texture = tex;
}

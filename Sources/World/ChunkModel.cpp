#include "World/ChunkModel.hpp"

#include <glad/glad.h>

World::ChunkModel::ChunkModel()
{
}

World::ChunkModel::~ChunkModel() {}

void World::ChunkModel::Render(Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp)
{
	if (!shaderProgram || mesh.GetVerticesCount() == 0) return;

	// Set matrix of model.
	Core::Maths::Mat4 mvp = vp * modelMat;
	glUniformMatrix4fv(shaderProgram->GetLocation(Resources::ShaderData::MVP), 1, GL_FALSE, mvp.content);
	glUniformMatrix4fv(shaderProgram->GetLocation(Resources::ShaderData::M), 1, GL_FALSE, modelMat.content);

	mesh.Draw(VAOCurrent);
}

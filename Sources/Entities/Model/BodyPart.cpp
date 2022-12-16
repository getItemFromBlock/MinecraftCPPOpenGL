#include "Entities/Model/BodyPart.hpp"

#include <glad/glad.h>

#include "Entities/Model/CubeModelVertice.hpp"

#include "Core/Debug/Log.hpp"

using namespace Entities::Model;

BodyPart::BodyPart()
{
}

BodyPart::~BodyPart()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	indexCount = 0;
}

BodyPart* BodyPart::texOffs(float u, float v)
{
	models.back().texOffset = Vec2(u, v);
	return this;
}

BodyPart* BodyPart::mirror()
{
	models.back().mirrored = true;
	return this;
}

BodyPart* BodyPart::addBox(float xo, float yo, float zo, float xs, float ys, float zs)
{
	models.push_back(CubeModel());
	models.back().offset = Vec3(xo, yo, zo);
	models.back().size = Vec3(xs, ys, zs);
	return this;
}

BodyPart* Entities::Model::BodyPart::extend(float val)
{
	models.back().extend = val;
	return this;
}

void BodyPart::Bake(Vec2 textSize)
{
	std::vector<CubeModelVertice> vertices;
	std::vector<unsigned int> indices;
	for (size_t i = 0; i < models.size(); i++)
	{
		CubeModel& model = models[i];
		Vec3 ps[8] =
		{
			-model.offset + Vec3(model.extend, model.extend, model.extend),
			-model.offset - Vec3(0, model.size.y, 0) + Vec3(model.extend, -model.extend, model.extend),
			-model.offset - Vec3(0, 0, model.size.z) + Vec3(model.extend, model.extend, -model.extend),
			-model.offset - Vec3(0, model.size.y, model.size.z) + Vec3(model.extend, -model.extend, -model.extend),
			-model.offset - Vec3(model.size.x, 0, 0) + Vec3(-model.extend, model.extend, model.extend),
			-model.offset - Vec3(model.size.x, model.size.y, 0) + Vec3(-model.extend, -model.extend, model.extend),
			-model.offset - Vec3(model.size.x, 0, model.size.z) + Vec3(-model.extend, model.extend, -model.extend),
			-model.offset - Vec3(model.size.x, model.size.y, model.size.z) + Vec3(-model.extend, -model.extend, -model.extend)
		};
		Vec3 ns[6] =
		{
			Vec3(1,0,0),
			Vec3(-1,0,0),
			Vec3(0,1,0),
			Vec3(0,-1,0),
			Vec3(0,0,1),
			Vec3(0,0,-1),
		};
		if (model.mirrored)
		{
			vertices.push_back(CubeModelVertice(ps[0], ns[0], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[1], ns[0], (Vec2(model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[2], ns[0], (Vec2(2 * model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[3], ns[0], (Vec2(2 * model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[6], ns[1], (Vec2(model.size.z, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[7], ns[1], (Vec2(model.size.z, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[4], ns[1], (Vec2(0, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[5], ns[1], (Vec2(0, model.size.z + model.size.y) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[4], ns[2], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[0], ns[2], (Vec2(model.size.z, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[6], ns[2], (Vec2(model.size.z + model.size.x, 0) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[2], ns[2], (Vec2(model.size.z, 0) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[1], ns[3], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[5], ns[3], (Vec2(model.size.z + 2 * model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[3], ns[3], (Vec2(model.size.z + model.size.x, 0) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[7], ns[3], (Vec2(model.size.z + 2 * model.size.x, 0) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[4], ns[4], (Vec2(2 * model.size.z + 2 * model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[5], ns[4], (Vec2(2 * model.size.z + 2 * model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[0], ns[4], (Vec2(2 * model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[1], ns[4], (Vec2(2 * model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[2], ns[5], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[3], ns[5], (Vec2(model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[6], ns[5], (Vec2(model.size.z, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[7], ns[5], (Vec2(model.size.z, model.size.z + model.size.y) + model.texOffset) / textSize));
		}
		else
		{
			vertices.push_back(CubeModelVertice(ps[0], ns[0], (Vec2(0, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[1], ns[0], (Vec2(0, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[2], ns[0], (Vec2(model.size.z, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[3], ns[0], (Vec2(model.size.z, model.size.z + model.size.y) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[6], ns[1], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[7], ns[1], (Vec2(model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[4], ns[1], (Vec2(2 * model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[5], ns[1], (Vec2(2 * model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[4], ns[2], (Vec2(model.size.z + model.size.x, 0) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[0], ns[2], (Vec2(model.size.z, 0) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[6], ns[2], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[2], ns[2], (Vec2(model.size.z, model.size.z) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[1], ns[3], (Vec2(model.size.z + model.size.x, 0) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[5], ns[3], (Vec2(model.size.z + 2 * model.size.x, 0) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[3], ns[3], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[7], ns[3], (Vec2(model.size.z + 2 * model.size.x, model.size.z) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[4], ns[4], (Vec2(2 * model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[5], ns[4], (Vec2(2 * model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[0], ns[4], (Vec2(2 * model.size.z + 2 * model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[1], ns[4], (Vec2(2 * model.size.z + 2 * model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));

			vertices.push_back(CubeModelVertice(ps[2], ns[5], (Vec2(model.size.z, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[3], ns[5], (Vec2(model.size.z, model.size.z + model.size.y) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[6], ns[5], (Vec2(model.size.z + model.size.x, model.size.z) + model.texOffset) / textSize));
			vertices.push_back(CubeModelVertice(ps[7], ns[5], (Vec2(model.size.z + model.size.x, model.size.z + model.size.y) + model.texOffset) / textSize));
		}
		for (int i = 0; i < 24; i++)
		{
			Vec2 tmp = vertices[vertices.size() - 1 - i].uv;
			tmp.y = 1 - tmp.y;
			vertices[vertices.size() - 1 - i].uv = tmp;
		}
		for (size_t n = 0; n < 6; n++)
		{
			indices.push_back(static_cast<unsigned int>(i * 24 + n * 4));
			indices.push_back(static_cast<unsigned int>(i * 24 + n * 4 + 1));
			indices.push_back(static_cast<unsigned int>(i * 24 + n * 4 + 2));
			indices.push_back(static_cast<unsigned int>(i * 24 + n * 4 + 2));
			indices.push_back(static_cast<unsigned int>(i * 24 + n * 4 + 1));
			indices.push_back(static_cast<unsigned int>(i * 24 + n * 4 + 3));
		}
	}
	indexCount = static_cast<unsigned int>(indices.size());
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(CubeModelVertice), vertices.data(), GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeModelVertice), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CubeModelVertice), (void*)(sizeof(Core::Maths::Vec3)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CubeModelVertice), (void*)(2 * sizeof(Core::Maths::Vec3)));
	glEnableVertexAttribArray(2);
}

void BodyPart::Render(unsigned int& VAOCurrent)
{
	if (!visible) return;
	if (VAOCurrent != VAO)
	{
		glBindVertexArray(VAO);
		VAOCurrent = VAO;
	}
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Entities::Model::BodyPart::copyFrom(BodyPart* other)
{
	offset = other->offset;
	rotation = other->rotation;
}

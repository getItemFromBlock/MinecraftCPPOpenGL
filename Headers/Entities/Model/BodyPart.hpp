#pragma once

#include <string>

#include "CubeModel.hpp"

namespace Entities::Model
{
	class BodyPart
	{
	public:
		BodyPart();
		~BodyPart();

		BodyPart* texOffs(float u, float v);
		BodyPart* mirror();
		BodyPart* addBox(float xo, float yo, float zo, float xs, float ys, float zs);
		BodyPart* extend(float val);
		void Bake();
		void Render(unsigned int& VAOCurrent);
		void copyFrom(BodyPart* other);

		Vec3 offset;
		Vec3 rotation;
		bool visible = true;
	private:
		unsigned int EBO = 0;
		unsigned int VBO = 0;
		unsigned int VAO = 0;
		unsigned int indexCount = 0;

		std::vector<CubeModel> models;
	};
}
#pragma once

#include <vector>

#include "ModelLoader.hpp"

namespace Core::Util
{
	enum class ShapeType
	{
		DEFAULT,
		EMPTY,
	};

	class Shape
	{
	public:
		std::vector<Core::Util::Vertice> ShapeData;

		void AddToChunk(Core::Maths::IVec3 blockPos, std::vector<Core::Util::Vertice>* vertices) const;

		static void CreateCubeShape(Shape in[6]);
		static void CreateFaceShape(char side, Shape& in, Core::Maths::Vec3 from, Core::Maths::Vec3 to, Core::Maths::Vec2 uvA, Core::Maths::Vec2 uvB);
	private:
	};
}
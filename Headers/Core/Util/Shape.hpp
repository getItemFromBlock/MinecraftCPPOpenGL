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

		void AddToChunk(Core::Maths::Int3D blockPos, std::vector<Core::Util::Vertice>* vertices) const;

		static void CreateCubeShape(Shape in[6]);
		static void CreateFaceShape(char side, Shape& in, Core::Maths::Vec3D from, Core::Maths::Vec3D to, Core::Maths::Vec2D uvA, Core::Maths::Vec2D uvB);
	private:
	};
}
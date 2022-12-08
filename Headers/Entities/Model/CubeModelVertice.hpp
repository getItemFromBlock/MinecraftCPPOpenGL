#pragma once

#include "Core/Maths/Maths.hpp"

using namespace Core::Maths;

namespace Entities::Model
{
	class CubeModelVertice
	{
	public:
		CubeModelVertice() {};
		CubeModelVertice(Vec3 position, Vec3 normal, Vec2 texCoord) : pos(position), nrm(normal), uv(texCoord) {};
		~CubeModelVertice() {};

		Vec3 pos;
		Vec3 nrm;
		Vec2 uv;
	private:

	};
}
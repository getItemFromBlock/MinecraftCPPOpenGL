#pragma once

#include "Core/Maths/Maths.hpp"

namespace Physics
{
	class Transform
	{
	public:
		Transform();
		~Transform();

		Core::Maths::Mat4 local = Core::Maths::Mat4::Identity();
		Core::Maths::Mat4 global = Core::Maths::Mat4::Identity();

		Core::Maths::Vec3 position = Core::Maths::Vec3();
		Core::Maths::Vec3 rotation = Core::Maths::Vec3();
		Core::Maths::Vec3 scale = Core::Maths::Vec3(1);

		void DrawGUI();

		void Update();
	private:

	};
}
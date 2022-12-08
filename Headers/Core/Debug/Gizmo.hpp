#pragma once

#include <vector>

#include "Core/Util/HitBox.hpp"

namespace Core::Debug
{
	struct ColoredBox
	{
		Core::Util::Box box;
		Core::Maths::Vec3 color;
		bool wireframe = false;
		ColoredBox(const Core::Util::Box& b, Core::Maths::Vec3 c, bool wire = false) : box(b), color(c), wireframe(wire) {}
	};
	class Gizmo
	{
	public:
		static void PushElement(const Core::Util::Box& box, Core::Maths::Vec3 color = Core::Maths::Vec3(1), bool wireframe = false);
		static void ClearElements();
		static size_t GetSize();
		static const Core::Debug::ColoredBox& GetItem(size_t index);
	private:
		static inline std::vector<ColoredBox> boxes;
	};
	class OverlayGizmo
	{
	public:
		static void PushElement(const Core::Util::Box& box, Core::Maths::Vec3 color = Core::Maths::Vec3(1), bool wireframe = false);
		static void ClearElements();
		static size_t GetSize();
		static const Core::Debug::ColoredBox& GetItem(size_t index);
	private:
		static inline std::vector<ColoredBox> boxes;
	};
}
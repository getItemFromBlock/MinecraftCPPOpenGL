#include "Core/Debug/Gizmo.hpp"

void Core::Debug::Gizmo::PushElement(const Core::Util::Box& box, Core::Maths::Vec3 color, bool wireframe)
{
	boxes.push_back(ColoredBox(box, color, wireframe));
}

void Core::Debug::Gizmo::ClearElements()
{
	boxes.clear();
}

size_t Core::Debug::Gizmo::GetSize()
{
	return boxes.size();
}

const Core::Debug::ColoredBox& Core::Debug::Gizmo::GetItem(size_t index)
{
	return boxes.at(index);
}

void Core::Debug::OverlayGizmo::PushElement(const Core::Util::Box& box, Core::Maths::Vec3 color, bool wireframe)
{
	boxes.push_back(ColoredBox(box, color, wireframe));
}

void Core::Debug::OverlayGizmo::ClearElements()
{
	boxes.clear();
}

size_t Core::Debug::OverlayGizmo::GetSize()
{
	return boxes.size();
}

const Core::Debug::ColoredBox& Core::Debug::OverlayGizmo::GetItem(size_t index)
{
	return boxes.at(index);
}

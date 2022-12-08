#include "Core/Util/HitBox.hpp"

void Core::Util::HitBox::CreateCubeHitBox(HitBox& in)
{
	in.HitBoxData.push_back(Core::Util::Box(Core::Maths::Vec3(0.5f), Core::Maths::Vec3(1.0f)));
}

bool Core::Util::Box::IsInside(Core::Maths::Vec3 pos) const
{
	return (pos.x >= (center.x - size.x / 2) && pos.x <= (center.x + size.x) &&
		pos.y >= (center.y - size.y / 2) && pos.y <= (center.y + size.y) &&
		pos.z >= (center.z - size.z / 2) && pos.z <= (center.z + size.z));
}

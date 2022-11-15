#include "Core/Util/HitBox.hpp"

void Core::Util::HitBox::CreateCubeHitBox(HitBox& in)
{
	in.HitBoxData.push_back(Core::Util::Box(Core::Maths::Vec3D(0.5f), Core::Maths::Vec3D(1.0f)));
}

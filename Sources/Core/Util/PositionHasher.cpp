#include "Core/Util/PositionHasher.hpp"

size_t Core::Util::PositionHasher::operator()(Core::Maths::IVec3 key) const
{
	size_t value = (size_t)(static_cast<uint32_t>(key.y) & 0xffff) |
		((size_t)(static_cast<uint32_t>(key.x) & 0xffffff) << 16) |
		((size_t)(static_cast<uint32_t>(key.z) & 0xffffff) << 40);
	return value;
}
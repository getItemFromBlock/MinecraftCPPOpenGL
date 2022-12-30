#include "Utils.hpp"

#include "Core/Debug/Assert.hpp"

namespace Networking::Utils
{
	void SetBit(uint64_t& bitfield, const uint8_t n)
	{
		Assert(n < 64);
		bitfield |= (Bit<uint64_t>::Right << n);
	}

	void UnsetBit(uint64_t& bitfield, const uint8_t n)
	{
		Assert(n < 64);
		bitfield &= (~(Bit<uint64_t>::Right << n));
	}

	bool HasBit(uint64_t bitfield, const uint8_t n)
	{
		Assert(n < 64);
		return (bitfield & (Bit<uint64_t>::Right << n)) != 0;
	}
}
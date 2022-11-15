#pragma once

#include <random>

namespace Core::Maths
{
	class PRNG
	{
	public:
		PRNG();
		PRNG(uint32_t seed);

		uint64_t nextLong();
		uint32_t nextInt();
		int32_t nextInt(int32_t min, int32_t max);
		float_t nextFloat();
		double_t nextDouble();
	private:
		std::mt19937 mtw;

	};
}
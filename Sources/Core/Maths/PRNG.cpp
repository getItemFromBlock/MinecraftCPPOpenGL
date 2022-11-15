#include "Core/Maths/PRNG.hpp"

#include <time.h>

std::uniform_int_distribution<uint32_t> uint_dist;
std::uniform_int_distribution<int32_t> int_dist;
std::uniform_int_distribution<uint64_t> uint64_dist;
std::normal_distribution<double> double_dist;
std::normal_distribution<float> float_dist;

Core::Maths::PRNG::PRNG()
{
	mtw.seed((uint32_t(time(nullptr))));
}

Core::Maths::PRNG::PRNG(uint32_t seed)
{
	mtw.seed(seed);
}

uint64_t Core::Maths::PRNG::nextLong()
{
	return uint64_dist(mtw);
}

uint32_t Core::Maths::PRNG::nextInt()
{
	return int_dist(mtw);
}

int32_t Core::Maths::PRNG::nextInt(int32_t min, int32_t max)
{
	std::uniform_int_distribution<int32_t> int_var_dist = std::uniform_int_distribution<int32_t>(min, max);
	return int_var_dist(mtw);
}

double Core::Maths::PRNG::nextDouble()
{
	return double_dist(mtw);
}

float Core::Maths::PRNG::nextFloat()
{
	return float_dist(mtw);
}
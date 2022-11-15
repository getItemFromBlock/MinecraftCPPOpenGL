#include "Blocks/Properties/IntegerProperty.hpp"

#include "Core/Debug/Assert.hpp"

int Blocks::Properties::IntegerProperty::Get() const
{
	return value;
}

void Blocks::Properties::IntegerProperty::Set(const int& in)
{
	Assert((in < max) && (in > 0));
	value = in;
}

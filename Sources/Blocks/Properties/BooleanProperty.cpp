#include "Blocks/Properties/BooleanProperty.hpp"

bool Blocks::Properties::BooleanProperty::Get() const
{
	return value;
}

void Blocks::Properties::BooleanProperty::Set(const bool& in)
{
	value = in;
}

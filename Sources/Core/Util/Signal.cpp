#include "Core/Util/Signal.hpp"

Core::Util::Signal::Signal() : value(false)
{
}

Core::Util::Signal::Signal(bool defaultValue) : value(defaultValue)
{
}

Core::Util::Signal::~Signal()
{
}

bool Core::Util::Signal::Store(bool valueIn)
{
	value.store(valueIn);
	return valueIn;
}

bool Core::Util::Signal::Load() const
{
	return value.load();
}

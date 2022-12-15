#pragma once

#include <atomic>

namespace Core::Util
{
	class Signal
	{
	public:
		Signal();
		Signal(bool defaultValue);
		~Signal();

		bool Store(bool valueIn);
		bool Load() const;
	private:
		std::atomic<bool> value;
	};
}

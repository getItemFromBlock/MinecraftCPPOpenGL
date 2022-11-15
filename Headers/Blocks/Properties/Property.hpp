#pragma once

namespace Blocks::Properties
{
	template <typename T>
	class Property
	{
	public:
		virtual int GetStateCount() = 0;
		virtual T Get() const = 0;
		virtual void Set(const T& in) = 0;
	protected:
	};
}
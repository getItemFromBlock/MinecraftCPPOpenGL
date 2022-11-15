#pragma once

#include "Property.hpp"

#include <string>

namespace Blocks::Properties
{
	enum class SlabType
	{
		TOP,
		BOTTOM,
		DOUBLE,
	};

	template <typename T>
	class EnumProperty : public Property<T>
	{
	public:
		EnumProperty(T defaultValue, int max_values) : value(defaultValue), max(max_values) {}
		~EnumProperty() {};

		int GetStateCount() override { return max; }
		T Get() const override { return value; }
		void Set(const T& in) { value = in; }
	protected:
		T value;
		int max;
	};
}
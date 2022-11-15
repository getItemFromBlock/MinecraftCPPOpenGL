#pragma once

#include "Property.hpp"

namespace Blocks::Properties
{
	class IntegerProperty : public Property<int>
	{
	public:
		IntegerProperty(int init, int max_values) : value(init), max(max_values) {}
		~IntegerProperty() {};

		int GetStateCount() override { return max; }
		int Get() const override;
		void Set(const int& in);
	protected:
		int value;
		int max;
	};
}
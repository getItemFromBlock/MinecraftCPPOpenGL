#pragma once

#include "Property.hpp"

namespace Blocks::Properties
{
	class BooleanProperty : public Property<bool>
	{
	public:
		BooleanProperty(bool init) : value(init) {}
		~BooleanProperty() {};

		int GetStateCount() override { return 2; }
		bool Get() const override;
		void Set(const bool& in);
	protected:
		bool value;
	};
}
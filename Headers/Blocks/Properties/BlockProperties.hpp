#pragma once

#include <vector>

class BlockProperties
{
public:
	BlockProperties();
	~BlockProperties();

private:
	std::vector<Blocks::Properties::BooleanProperty> properties;
};

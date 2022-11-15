#include "Physics/RigidBody.hpp"

#include <ImGUI/imgui.h>

#include "Core/Util/TextHelper.hpp"

#define TERMINAL_VEL 50.0f

Physics::RigidBody::RigidBody()
{
}

Physics::RigidBody::~RigidBody()
{
}

void Physics::RigidBody::DeleteBody()
{
	this->~RigidBody();
}
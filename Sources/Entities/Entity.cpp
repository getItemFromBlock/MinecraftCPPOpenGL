#include "Entities/Entity.hpp"
#include "Core/App/App.hpp"

Entities::Entity::Entity()
{
	uuid = Core::App::App::GetRNG().nextLong();
}

void Entities::Entity::Update(float deltatime)
{
}

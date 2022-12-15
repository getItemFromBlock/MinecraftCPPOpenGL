#include "Entities/Entity.hpp"
#include "Core/App/App.hpp"

Resources::ResourceManager* Entities::Entity::manager;

Entities::Entity::Entity()
{
	uuid = Core::App::App::GetRNG().nextLong();
}

void Entities::Entity::Update(float deltatime)
{
}

void Entities::Entity::SetResourceManager(Resources::ResourceManager* in)
{
	manager = in;
}

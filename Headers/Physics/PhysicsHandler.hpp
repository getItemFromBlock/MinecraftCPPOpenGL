#pragma once

#include <vector>
#include <string>
#include "Core/Maths/Maths.hpp"
#include "Blocks/Block.hpp"
#include "World/World.hpp"
#include "Entities/EntityLivingBase.hpp"

namespace Core::App
{
	class App;
}

namespace Physics
{
	namespace Colliders
	{
		class Collider;
	}

	struct RayCastResult
	{
		bool hit = false;
		Blocks::Block* target = nullptr;
		Core::Maths::Int3D position;
		Core::Maths::Int3D normal;
	};

	class PhysicsHandler
	{
	friend class Colliders::Collider;
	public:
		PhysicsHandler();
		~PhysicsHandler();

		static void UpdatePhysics(std::vector<Blocks::Block*>* blocks, std::vector<Core::Maths::Int3D>* poss, Entities::EntityLivingBase* entity);

		static RayCastResult RayCastBlock(World::World* worldIn, Core::Maths::Vec3D pos, Core::Maths::Vec3D dest);
	private:
	};
}
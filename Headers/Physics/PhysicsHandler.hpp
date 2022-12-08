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

	struct BlockRayCastResult
	{
		bool hit = false;
		Blocks::Block* target = nullptr;
		Core::Maths::IVec3 position;
		Core::Maths::IVec3 normal;
		Core::Maths::Vec3 point;
		float distance = INFINITY;
	};

	struct EntityRayCastResult
	{
		bool hit = false;
		Entities::EntityLivingBase* target = nullptr;
		Core::Maths::IVec3 normal;
		Core::Maths::Vec3 point;
		float distance = INFINITY;
	};

	struct AABB
	{
		Vec3 min;
		Vec3 max;
	};

	class PhysicsHandler
	{
	friend class Colliders::Collider;
	public:
		PhysicsHandler();
		~PhysicsHandler();

		static void UpdatePhysics(std::vector<Blocks::Block*>* blocks, std::vector<Core::Maths::IVec3>* poss, Entities::EntityLivingBase* entity);

		static void UpdateEntityPhysics(std::unordered_map<size_t, Entities::EntityLivingBase*>& entities, std::unordered_map<size_t, Entities::EntityLivingBase*>::iterator iter, float deltaTime);

		static void UpdateEntityPhysics(std::unordered_map<size_t, Entities::EntityLivingBase*>& entities, Entities::EntityLivingBase* entity, float deltaTime);

		static BlockRayCastResult RayCastBlock(World::World* worldIn, Core::Maths::Vec3 pos, Core::Maths::Vec3 dest);

		static EntityRayCastResult RayCastEntity(World::World* worldIn, Core::Maths::Vec3 pos, Core::Maths::Vec3 dest, Entities::EntityLivingBase* self = nullptr);

		static bool RayIntersectsAABB(Vec3 origin, Vec3 destination, AABB box, Vec3& point);
	private:
	};
}
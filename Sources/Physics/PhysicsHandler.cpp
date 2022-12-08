#include "Physics/PhysicsHandler.hpp"

#include <ImGUI/imgui.h>

#include "Core/Util/TextHelper.hpp"
#include "Core/Debug/Gizmo.hpp"

#include "Physics/RigidBody.hpp"

#define SPHERE_SPHERE 0x11
#define SPHERE_CUBE 0x21
#define CUBE_SPHERE 0x12
#define CUBE_CUBE 0x22

using namespace Core::Maths;

void Physics::PhysicsHandler::UpdatePhysics(std::vector<Blocks::Block*>* blocks, std::vector<IVec3>* poss, Entities::EntityLivingBase* entity)
{
	Vec3 entitySize = entity->GetHitBox().size;
	Vec3& entityPos = entity->Position;
	Vec3 entityHPos;
	bool wasGrounded = entity->OnGround;
	entity->OnGround = false;
	for (size_t i = 0; i < blocks->size(); i++)
	{
		if (blocks->at(i)->GetCollisionType() != Core::Util::CollisionType::DEFAULT) continue;
		const Core::Util::HitBox& bht = blocks->at(i)->GetHitBox();
		//Core::Debug::OverlayGizmo::PushElement(Core::Util::Box(Vec3(0.5) + poss->at(i), 0.25f), Vec3(0.1f,0.1f,0.9f));
		for (size_t h = 0; h < bht.HitBoxData.size(); h++)
		{
			entityHPos = entity->GetHitBox().center + entityPos;
			Vec3 hPos = bht.HitBoxData.at(h).center + poss->at(i);
			Vec3 hSize = bht.HitBoxData.at(h).size;
			
			Vec3 distance = entityHPos - hPos;
			Vec3 relativeDistance = Vec3((hSize.x + entitySize.x) / 2 - fabsf(distance.x), (hSize.y + entitySize.y) / 2 - fabsf(distance.y), (hSize.z + entitySize.z) / 2 - fabsf(distance.z));
			if (relativeDistance.x > 0 && relativeDistance.y > 0 && relativeDistance.z > 0)
			{
				float min = INFINITY;
				char axis = -1;
				for (char i = 0; i < 6; i++)
				{
					float dist = (i > 2 ? -distance[i%3] : distance[i%3]) + (hSize[i%3] + entitySize[i%3])/2;
					if (dist < min)
					{
						min = dist;
						axis = i;
					}
				}
				if (axis != 4 && min < 0.001f) return;
				bool test = wasGrounded || (relativeDistance.x > 1e-2f && relativeDistance.z > 1e-2f);
				if (distance.y > 0.0f && (hSize.y + entitySize.y)/2 - distance.y < 0.565f && test)
				{
					entityPos.y = hPos.y + hSize.y/2.0f;
					entity->Velocity.y = 0;
					entity->OnGround = true;
					continue;
				}
				else if (axis == 1 && test)
				{
					entityPos.y = hPos.y - hSize.y / 2 - entitySize.y - 0.001f;
					entity->Velocity.y = 0;
					continue;
				}
				else
				{
					entityPos[axis%3] = (axis < 3) ? (hPos[axis %3] - (hSize[axis %3] + entitySize[axis%3]) / 2.0f) - 0.001f : (hPos[axis %3] + (hSize[axis %3] + entitySize[axis %3]) / 2.0f) + 0.001f;
					entity->Velocity[axis %3] = 0.0f;
					continue;
				}
			}
		}
	}
}

void Physics::PhysicsHandler::UpdateEntityPhysics(std::unordered_map<size_t, Entities::EntityLivingBase*>& entities, std::unordered_map<size_t, Entities::EntityLivingBase*>::iterator iter, float deltaTime)
{
	Entities::EntityLivingBase* entity = iter->second;
	iter++;
	Core::Util::Box b1 = entity->GetHitBox();
	Vec3 hPos1 = entity->Position + b1.center;
	Vec3 hsize1 = b1.size / 2.0f;
	Vec3 min1 = hPos1 - hsize1;
	Vec3 max1 = hPos1 + hsize1;
	for (iter; iter != entities.end(); iter++)
	{
		Entities::EntityLivingBase* e = iter->second;
		if (!e || e == entity || (fabsf(e->Position.x - entity->Position.x) < 1e-5f && fabsf(e->Position.z - entity->Position.z) < 1e-5f)) continue;
		Core::Util::Box b2 = e->GetHitBox();
		Vec3 hPos2 = e->Position + b1.center;
		Vec3 hsize2 = b2.size / 2.0f;
		Vec3 max2 = hPos2 + hsize2;
		Vec3 min2 = hPos2 - hsize2;
		if (max1.x <= min2.x || max2.x <= min1.x || max1.y <= min2.y || max2.y <= min1.y || max1.z <= min2.z || max2.z <= min1.z) continue;
		/*
		Vec3 relativePos;
		relativePos[0] = hPos1[2] - hPos2[0] / (hsize1[2] + hsize2[2]);
		relativePos[2] = hPos1[0] - hPos2[0] / (hsize1[2] + hsize2[2]);
		float factor = 1 - relativePos.getLength() / 2.0f;
		*/
		Vec3 direction = entity->Position - e->Position;
		direction.y = 0.0f;
		direction = direction.unitVector();
		entity->Velocity = entity->Velocity + direction * 0.01f * deltaTime;
		e->Velocity = e->Velocity - direction * 0.01f * deltaTime;
	}
}

void Physics::PhysicsHandler::UpdateEntityPhysics(std::unordered_map<size_t, Entities::EntityLivingBase*>& entities, Entities::EntityLivingBase* entity, float deltaTime)
{ 
	Core::Util::Box b1 = entity->GetHitBox();
	Vec3 min1 = entity->Position + b1.center - b1.size / 2.0f;
	Vec3 max1 = entity->Position + b1.center + b1.size / 2.0f;
	for (auto &pair : entities)
	{
		Entities::EntityLivingBase* e = pair.second;
		if (!e || e == entity || e->Position.isNearlyEqual(entity->Position) || (e->riding && e->ridingEntity == entity) || (entity->riding && entity->ridingEntity == e)) continue;
		Core::Util::Box b2 = e->GetHitBox();
		Vec3 max2 = e->Position + b2.center + b2.size / 2.0f;
		Vec3 min2 = e->Position + b2.center - b2.size / 2.0f;
		if (max1.x <= min2.x || max2.x <= min1.x || max1.y <= min2.y || max2.y <= min1.y || max1.z <= min2.z || max2.z <= min1.z) continue;
	}
}

Physics::BlockRayCastResult Physics::PhysicsHandler::RayCastBlock(World::World* worldIn, Vec3 pos, Vec3 dest)
{
	BlockRayCastResult result;
	result.point = dest;
	Vec3 direction = dest - pos;
	IVec3 blockRayPos = IVec3((int)roundf(pos.x - 0.5f), (int)roundf(pos.y - 0.5f), (int)roundf(pos.z - 0.5f));
	Blocks::Block* t = worldIn->GetBlockAt(blockRayPos);
	if (t && t->GetCollisionType() != Core::Util::CollisionType::NONE)
	{
		for (size_t i = 0; i < t->GetHitBox().HitBoxData.size(); i++)
		{
			const Core::Util::Box& b = t->GetHitBox().HitBoxData.at(i);
			if (b.IsInside(pos - blockRayPos))
			{
				result.position = blockRayPos;
				result.target = t;
				result.hit = true;
				result.normal = IVec3(0, 1, 0);
				Core::Util::Box tmp = b;
				tmp.center = tmp.center + result.position;
				tmp.size = tmp.size + 0.0001f;
				Core::Debug::Gizmo::PushElement(tmp, Vec3(), true);
				return result;
			}
		}
	}
	Vec3 dir = (dest - pos).unitVector();
	IVec3 minBlock = IVec3((int)Util::minF(pos.x, dest.x), (int)Util::minF(pos.y, dest.y), (int)Util::minF(pos.z, dest.z));
	IVec3 maxBlock = IVec3((int)Util::maxF(pos.x, dest.x), (int)Util::maxF(pos.y, dest.y), (int)Util::maxF(pos.z, dest.z));
	for (int i = minBlock.x - 1; i < maxBlock.x + 1; i++)
		for (int j = minBlock.y - 1; j < maxBlock.y + 1; j++)
			for (int k = minBlock.z - 1; k < maxBlock.z + 1; k++)
			{
				IVec3 blockPos = IVec3(i, j, k);
				t = worldIn->GetBlockAt(blockPos);
				if (!t) continue;
				const Core::Util::HitBox& box = t->GetHitBox();
				if (t->GetCollisionType() == Core::Util::CollisionType::NONE) continue;
				for (size_t s = 0; s < box.HitBoxData.size(); s++)
				{
					const Core::Util::Box& b = box.HitBoxData.at(s);
					for (char a = 0; a < 6; a++)
					{
						if (a < 3 ? (dir[a % 3] >= 0.0f) : (dir[a % 3] <= 0.0f)) continue;
						float l = (b.center[a % 3] + blockPos[a%3]) + (a < 3 ? (b.size[a % 3]/2 - pos[a % 3]) : (-b.size[a % 3]/2 - pos[a % 3]));
						l = l / dir[a % 3];
						Vec3 point = pos + dir * l;
						if ((point[(a + 1) % 3] < blockPos[(a + 1) % 3] + b.center[(a + 1) % 3] - b.size[(a + 1) % 3] / 2) ||
							(point[(a + 1) % 3]) > (blockPos[(a + 1) % 3] + b.center[(a + 1) % 3] + b.size[(a + 1) % 3] / 2) ||
							(point[(a + 2) % 3] < blockPos[(a + 2) % 3] + b.center[(a + 2) % 3] - b.size[(a + 2) % 3] / 2) ||
							(point[(a + 2) % 3]) > (blockPos[(a + 2) % 3] + b.center[(a + 2) % 3] + b.size[(a + 2) % 3] / 2)) continue;
						Vec3 pointdir = point - pos;
						if (pointdir.lengthSquared() >= (result.point - pos).lengthSquared() || direction.dotProduct(pointdir) <= 0.0f) continue;
						result.point = point;
						result.hit = true;
						result.target = t;
						result.position = blockPos;
						IVec3 normal;
						normal[a % 3] = a < 3 ? 1 : -1;
						result.normal = normal;
						result.distance = (result.point - pos).getLength();
					}
				}
			}
	if (result.hit)
	{
		for (size_t i = 0; i < result.target->GetHitBox().HitBoxData.size(); i++)
		{
			Core::Util::Box tmp = result.target->GetHitBox().HitBoxData.at(i);
			tmp.center = tmp.center + result.position;
			tmp.size = tmp.size + 0.0001f;
			Core::Debug::Gizmo::PushElement(tmp, Vec3() , true);
		}
	}
	return result;
}

Physics::EntityRayCastResult Physics::PhysicsHandler::RayCastEntity(World::World* worldIn, Vec3 pos, Vec3 dest, Entities::EntityLivingBase* self)
{
	EntityRayCastResult result;
	result.point = dest;
	Vec3 direction = dest - pos;
	Vec3 dir = (dest - pos).unitVector();
	for (auto t = worldIn->entities.begin(); t != worldIn->entities.end(); t++)
	{
		Entities::EntityLivingBase* entity = t->second;
		if (entity == self) continue;
		if (!entity) continue;
		const Core::Util::Box& box = entity->GetHitBox();
		if (box.IsInside(pos - entity->Position))
		{
			result.point = pos;
			result.hit = true;
			result.target = entity;
			result.normal = Vec3(0, 1, 0);
			result.distance = 0.0f;
			Core::Util::Box tmp = box;
			tmp.center = tmp.center + entity->Position;
			tmp.size = tmp.size + 0.0001f;
			Core::Debug::Gizmo::PushElement(tmp, Vec3(), true);
			return result;
		}
		for (char a = 0; a < 6; a++)
		{
			if (a < 3 ? (dir[a % 3] >= 0.0f) : (dir[a % 3] <= 0.0f)) continue;
			float l = (box.center[a % 3] + entity->Position[a % 3]) + (a < 3 ? (box.size[a % 3] / 2 - pos[a % 3]) : (-box.size[a % 3] / 2 - pos[a % 3]));
			l = l / dir[a % 3];
			Vec3 point = pos + dir * l;
			if ((point[(a + 1) % 3] < entity->Position[(a + 1) % 3] + box.center[(a + 1) % 3] - box.size[(a + 1) % 3] / 2) ||
				(point[(a + 1) % 3]) > (entity->Position[(a + 1) % 3] + box.center[(a + 1) % 3] + box.size[(a + 1) % 3] / 2) ||
				(point[(a + 2) % 3] < entity->Position[(a + 2) % 3] + box.center[(a + 2) % 3] - box.size[(a + 2) % 3] / 2) ||
				(point[(a + 2) % 3]) > (entity->Position[(a + 2) % 3] + box.center[(a + 2) % 3] + box.size[(a + 2) % 3] / 2)) continue;
			Vec3 pointdir = point - pos;
			if (pointdir.lengthSquared() >= (result.point - pos).lengthSquared() || direction.dotProduct(pointdir) <= 0.0f) continue;
			result.point = point;
			result.hit = true;
			result.target = entity;
			IVec3 normal;
			normal[a % 3] = a < 3 ? 1 : -1;
			result.normal = normal;
			result.distance = (result.point - pos).getLength();
		}
	}
	if (result.hit)
	{
		Core::Util::Box tmp = result.target->GetHitBox();
		tmp.center = tmp.center + result.target->Position;
		tmp.size = tmp.size + 0.0001f;
		Core::Debug::Gizmo::PushElement(tmp, Vec3(), true);
	}
	return result;
}

bool Physics::PhysicsHandler::RayIntersectsAABB(Vec3 origin, Vec3 destination, AABB box, Vec3& point) {
	float d0 = (box.min.x + box.max.x) * 0.5f;
	float d1 = (box.max.x - box.min.x) * 0.5f;
	float d2 = origin.x - d0;
	if (fabsf(d2) > d1 && d2 * destination.x >= 0.0f) {
		return false;
	}
	else {
		float d3 = (box.min.y + box.max.y) * 0.5f;
		float d4 = (box.max.y - box.min.y) * 0.5f;
		float d5 = origin.y - d3;
		if (fabsf(d5) > d4 && d5 * destination.y >= 0.0f) {
			return false;
		}
		else {
			float d6 = (box.min.z + box.max.z) * 0.5f;
			float d7 = (box.max.z - box.min.z) * 0.5f;
			float d8 = origin.z - d6;
			if (fabsf(d8) > d7 && d8 * destination.z >= 0.0f) {
				return false;
			}
			else {
				float d9 = fabsf(destination.x);
				float d10 = fabsf(destination.y);
				float d11 = fabsf(destination.z);
				float d12 = destination.y * d8 - destination.z * d5;
				if (fabsf(d12) > d4 * d11 + d7 * d10) {
					return false;
				}
				else {
					d12 = destination.z * d2 - destination.x * d8;
					if (fabsf(d12) > d1 * d11 + d7 * d9) {
						return false;
					}
					else {
						d12 = destination.x * d5 - destination.y * d2;
						bool a = fabsf(d12) < d1 * d10 + d4 * d9;
						if (!a) return false;
						point = Vec3(d12, d5, d8);
						return true;
					}
				}
			}
		}
	}
}

Physics::PhysicsHandler::PhysicsHandler()
{
}

Physics::PhysicsHandler::~PhysicsHandler()
{
}
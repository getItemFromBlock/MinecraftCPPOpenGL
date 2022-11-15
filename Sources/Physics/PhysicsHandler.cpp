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

void Physics::PhysicsHandler::UpdatePhysics(std::vector<Blocks::Block*>* blocks, std::vector<Core::Maths::Int3D>* poss, Entities::EntityLivingBase* entity)
{
	Core::Maths::Vec3D entitySize = entity->GetHitBox().size;
	Core::Maths::Vec3D& entityPos = entity->Position;
	Core::Maths::Vec3D entityHPos;
	entity->OnGround = false;
	for (size_t i = 0; i < blocks->size(); i++)
	{
		if (blocks->at(i)->GetCollisionType() != Core::Util::CollisionType::DEFAULT) continue;
		const Core::Util::HitBox& bht = blocks->at(i)->GetHitBox();
		//Core::Debug::OverlayGizmo::PushElement(Core::Util::Box(Core::Maths::Vec3D(0.5) + poss->at(i), 0.25f), Core::Maths::Vec3D(0.1f,0.1f,0.9f));
		for (size_t h = 0; h < bht.HitBoxData.size(); h++)
		{
			entityHPos = entity->GetHitBox().center + entityPos;
			Core::Maths::Vec3D hPos = bht.HitBoxData.at(h).center + poss->at(i);
			Core::Maths::Vec3D hSize = bht.HitBoxData.at(h).size;
			
			Core::Maths::Vec3D distance = entityHPos - hPos;
			if (fabsf(distance.x) < (hSize.x + entitySize.x)/2 && fabsf(distance.z) < (hSize.z + entitySize.z)/2 && fabsf(distance.y) < (hSize.y + entitySize.y)/2)
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
				if (distance.y > 0.0f && (hSize.y + entitySize.y)/2 - distance.y < 0.565f)
				{
					entityPos.y = hPos.y + hSize.y/2.0f;
					entity->Velocity.y = 0;
					entity->OnGround = true;
					continue;
				}
				else if (axis == 1)
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

Physics::RayCastResult Physics::PhysicsHandler::RayCastBlock(World::World* worldIn, Core::Maths::Vec3D pos, Core::Maths::Vec3D dest)
{
	RayCastResult result;
	Blocks::Block* t = worldIn->GetBlockAt(Core::Maths::Int3D(pos));
	if (t && t->GetCollisionType() != Core::Util::CollisionType::NONE)
	{
		for (size_t i = 0; i < t->GetHitBox().HitBoxData.size(); i++)
		{
			const Core::Util::Box& b = t->GetHitBox().HitBoxData.at(i);
			if (pos.x >= (b.center.x + (int)(pos.x) - (pos.x < 0) - b.size.x/2) && pos.x <= (b.center.x + (int)(pos.x/2) - (pos.x < 0) + b.size.x) &&
				pos.y >= (b.center.y + (int)(pos.y) - (pos.y < 0) - b.size.y/2) && pos.y <= (b.center.y + (int)(pos.y/2) - (pos.y < 0) + b.size.y) &&
				pos.z >= (b.center.z + (int)(pos.z) - (pos.z < 0) - b.size.z/2) && pos.z <= (b.center.z + (int)(pos.z/2) - (pos.z < 0) + b.size.z))
			{
				result.position = pos;
				result.target = t;
				result.hit = true;
				result.normal = Core::Maths::Int3D(0, 1, 0);
				return result;
			}
		}
	}
	Core::Maths::Vec3D dir = (dest - pos).unitVector();
	Core::Maths::Int3D minBlock = Core::Maths::Int3D((int)Core::Maths::Util::minF(pos.x, dest.x), (int)Core::Maths::Util::minF(pos.y, dest.y), (int)Core::Maths::Util::minF(pos.z, dest.z));
	Core::Maths::Int3D maxBlock = Core::Maths::Int3D((int)Core::Maths::Util::maxF(pos.x, dest.x), (int)Core::Maths::Util::maxF(pos.y, dest.y), (int)Core::Maths::Util::maxF(pos.z, dest.z));
	for (int i = minBlock.x - 1; i < maxBlock.x + 1; i++)
		for (int j = minBlock.y - 1; j < maxBlock.y + 1; j++)
			for (int k = minBlock.z - 1; k < maxBlock.z + 1; k++)
			{
				Core::Maths::Int3D blockPos = Core::Maths::Int3D(i, j, k);
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
						Core::Maths::Vec3D point = pos + dir * l;
						if ((point[(a + 1) % 3] < blockPos[(a + 1) % 3] + b.center[(a + 1) % 3] - b.size[(a + 1) % 3] / 2) ||
							(point[(a + 1) % 3]) > (blockPos[(a + 1) % 3] + b.center[(a + 1) % 3] + b.size[(a + 1) % 3] / 2) ||
							(point[(a + 2) % 3] < blockPos[(a + 2) % 3] + b.center[(a + 2) % 3] - b.size[(a + 2) % 3] / 2) ||
							(point[(a + 2) % 3]) > (blockPos[(a + 2) % 3] + b.center[(a + 2) % 3] + b.size[(a + 2) % 3] / 2)) continue;
						if ((point - pos).lengthSquared() >= (dest - pos).lengthSquared()) continue;
						dest = point;
						result.hit = true;
						result.target = t;
						result.position = blockPos;
						Core::Maths::Int3D normal;
						normal[a % 3] = a < 3 ? 1 : -1;
						result.normal = normal;
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
			Core::Debug::Gizmo::PushElement(tmp, Core::Maths::Vec3D() , true);
		}
	}
	return result;
}

Physics::PhysicsHandler::PhysicsHandler()
{
}

Physics::PhysicsHandler::~PhysicsHandler()
{
}
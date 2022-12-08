#pragma once

#include "Core/Maths/Maths.hpp"

namespace Physics
{
	class PhysicsHandler;
	class RigidBody
	{
		friend class PhysicsHandler;
	public:
		RigidBody();
		~RigidBody();
		
		//void PhysicUpdate(Core::DataStructure::Node* container, std::vector<Physics::Colliders::Collider*>* colliders, float deltaTime) override;

		void DeleteBody();

		Core::Maths::Vec3 Gravity = Core::Maths::Vec3(0,-2.0f,0);
		float Mass = 1.0f;
		float Bounciness = 0.5f;
		float DynamicFriction = 0.5f;
		float StaticFriction = 0.03f;

		Core::Maths::Vec3 Velocity = Core::Maths::Vec3();
		Core::Maths::Vec3 RotVelocity = Core::Maths::Vec3();
	private:

	};
}
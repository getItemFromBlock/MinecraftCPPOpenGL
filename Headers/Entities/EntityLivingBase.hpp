#pragma once

#include "Entity.hpp"

#include "Core/Util/HitBox.hpp"

namespace Entities
{
	class EntityLivingBase : public Entity
	{
	public:
		bool OnGround;
		float Health;
		float MaxHealth;
		float Rotation;
		float EyeHeight;
		float MovementSpeed;
		Core::Maths::Vec3D Velocity;
		Core::Maths::Vec3D ViewRotation;

		EntityLivingBase();

		virtual void Update(float deltatime) override;
		virtual void DamageEntity(float Amount);
		virtual void OnDeath();

		const Core::Util::Box& GetHitBox() { return HitBox; }
	protected:
		Core::Util::Box HitBox;
	};
}
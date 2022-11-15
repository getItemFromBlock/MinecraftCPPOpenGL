#include "Entities\EntityLivingBase.hpp"

#define GRAVITY 0.023f

void Entities::EntityLivingBase::Update(float deltatime)
{
	Velocity.y -= GRAVITY * deltatime;
	Position = Position + Velocity * deltatime * 1000;
}

Entities::EntityLivingBase::EntityLivingBase()
{
	OnGround = true;
	MaxHealth = 20.0f;
	Health = 20.0f;
	MovementSpeed = 0.005f;
	Rotation = 0.0f;
	EyeHeight = 1.75f;
}

void Entities::EntityLivingBase::DamageEntity(float amount)
{
	if (amount < 0.0f) amount = 0.0f;
	Health -= amount;
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		OnDeath();
	}
}

void Entities::EntityLivingBase::OnDeath()
{
}

#include "Entities\EntityLivingBase.hpp"

#define GRAVITY 0.023f

void Entities::EntityLivingBase::Update(float deltatime)
{
	if (!riding || !ridingEntity)
	{
		ridingEntity = nullptr;
		float tmp = Velocity.y;
		Velocity = Velocity + MovementOut * deltatime * (OnGround ? MovementSpeed * 15.0f : MovementSpeed * 1.5f) * effectiveSpeed;
		Velocity = Velocity * (OnGround ? 1.0f - 10.0f * deltatime : 1.0f - 1.0f * deltatime);
		if (OnGround)
		{
			tmp = 0;
		}
		tmp -= GRAVITY * deltatime;
		Velocity.y = tmp;
		Position = Position + Velocity * deltatime * 1000;
	}
	else
	{
		Velocity = Vec3();
		Position = ridingEntity->getRidePosition(this);
		if (ridingEntity->Health <= 0.0f)
		{
			riding = false;
			ridingEntity = nullptr;
		}
	}
	Rotation = ViewRotation.y;
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

Entities::EntityLivingBase::~EntityLivingBase()
{
	model.DeleteModel();
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

void Entities::EntityLivingBase::Jump()
{
	if (OnGround)
	{
		Velocity.y += 0.008f;
		OnGround = false;
	}
}

void Entities::EntityLivingBase::Render(World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp, bool IsShadowMap)
{
	model.Render(Position + Vec3(0, 1.5f, 0), Rotation, worldIn, shaderProgram, VAOCurrent, vp);
}

int Entities::EntityLivingBase::getFallFlyingTicks()
{
	return 0;
}

bool Entities::EntityLivingBase::isVisuallySwimming()
{
	return false;
}

float Entities::EntityLivingBase::rotlerpRad(float alpha, float min, float max) {
	float f = fmodf(max - min, (float)M_PI * 2.0f);
	if (f < -(float)M_PI) {
		f += ((float)M_PI * 2.0F);
	}

	if (f >= (float)M_PI) {
		f -= ((float)M_PI * 2.0F);
	}

	return min + alpha * f;
}

Vec3 Entities::EntityLivingBase::getRidePosition(EntityLivingBase* entity)
{
	return Position + Vec3(0, HitBox.size.y, 0);
}

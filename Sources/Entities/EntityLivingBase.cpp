#include "Entities\EntityLivingBase.hpp"

#define GRAVITY 0.023f

void Entities::EntityLivingBase::Update(float deltatime)
{
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

float Entities::EntityLivingBase::rotlerpRad(float p_102836_, float p_102837_, float p_102838_) {
	float f = fmodf(p_102838_ - p_102837_, (float)M_PI * 2.0f);
	if (f < -(float)M_PI) {
		f += ((float)M_PI * 2.0F);
	}

	if (f >= (float)M_PI) {
		f -= ((float)M_PI * 2.0F);
	}

	return p_102837_ + p_102836_ * f;
}
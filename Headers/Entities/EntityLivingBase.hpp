#pragma once

#include "Entity.hpp"
#include "Model/EntityModel.hpp"
#include "Resources/Texture.hpp"
#include "Core/Util/HitBox.hpp"

namespace Physics
{
	class PhysicsHandler;
}

namespace Entities
{
	class EntityLivingBase : public Entity
	{
	public:
		bool active = true;
		bool OnGround;
		float Health;
		float MaxHealth;
		float Rotation;
		float EyeHeight;
		float MovementSpeed;
		float effectiveSpeed = 1.0f;
		Core::Maths::Vec3 MovementOut;
		Core::Maths::Vec3 Velocity;
		Core::Maths::Vec3 ViewRotation;

		EntityLivingBase();
		~EntityLivingBase();

		virtual void Update(float deltatime) override;
		virtual void DamageEntity(float Amount);
		virtual void OnDeath();
		virtual void Jump();
		virtual void SetupAnim(float deltaTime, float globalTime) = 0;
		virtual void Render(World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp, bool IsShadowMap);

		int getFallFlyingTicks();
		bool isVisuallySwimming();
		virtual Vec3 getRidePosition(EntityLivingBase* entity);

		const Core::Util::Box& GetHitBox() { return HitBox; }
	protected:
		Core::Util::Box HitBox;
		Model::EntityModel model;
		
		float swimAmount = 0.0f;
		float animCounter = 0.0f;
		bool riding = false;
		EntityLivingBase* ridingEntity = nullptr;
		float rotlerpRad(float a, float b, float c);
		Resources::Texture entityTexture;

		friend class Physics::PhysicsHandler;
	};
}
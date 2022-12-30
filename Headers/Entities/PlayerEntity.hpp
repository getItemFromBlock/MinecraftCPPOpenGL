#pragma once

#include "EntityLivingBase.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Model/ArmPose.hpp"

namespace Entities
{
	class PlayerEntity : public EntityLivingBase
	{
	public:

		PlayerEntity(const char* name = "", bool isSlim = false);

		virtual void Update(float deltatime) override;
		virtual void DamageEntity(float Amount) override;
		virtual void OnDeath() override;
		virtual void Render(World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp, bool IsShadowMap) override;
		virtual void SetupAnim(float deltaTime, float globalTime) override;
		virtual Vec3 getRidePosition(EntityLivingBase* entity) override;
	protected:
		Model::BodyPart* head = nullptr;
		Model::BodyPart* hat = nullptr;
		Model::BodyPart* body = nullptr;
		Model::BodyPart* rightArm = nullptr;
		Model::BodyPart* leftArm = nullptr;
		Model::BodyPart* rightLeg = nullptr;
		Model::BodyPart* leftLeg = nullptr;

		Model::BodyPart* leftSleeve = nullptr;
		Model::BodyPart* rightSleeve = nullptr;
		Model::BodyPart* leftPants = nullptr;
		Model::BodyPart* rightPants = nullptr;
		Model::BodyPart* jacket = nullptr;
		Model::BodyPart* cloak = nullptr;

		ArmPose rightArmPose = ArmPose::ITEM;
		ArmPose leftArmPose = ArmPose::EMPTY;

		bool leftHanded = false;
		bool crouching = false;
		bool attacking = false;
		float attackTime = 0.0f;
		bool isUsingItem();
		float quadraticArmUpdate(float value);
		void poseRightArm();
		void poseLeftArm();
		void setupAttackAnimation();
		void bobModelPart(Model::BodyPart* part, float globalTime, float factor);
	};
}
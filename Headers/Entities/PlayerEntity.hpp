#pragma once

#include "EntityLivingBase.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Model/ArmPose.hpp"
#include "Resources/PlayerTexture.hpp"

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
	protected:
		Model::BodyPart* head;
		Model::BodyPart* hat;
		Model::BodyPart* body;
		Model::BodyPart* rightArm;
		Model::BodyPart* leftArm;
		Model::BodyPart* rightLeg;
		Model::BodyPart* leftLeg;

		Model::BodyPart* leftSleeve;
		Model::BodyPart* rightSleeve;
		Model::BodyPart* leftPants;
		Model::BodyPart* rightPants;
		Model::BodyPart* jacket;
		Model::BodyPart* cloak;

		ArmPose rightArmPose = ArmPose::ITEM;
		ArmPose leftArmPose = ArmPose::EMPTY;

		Resources::PlayerTexture* texture = nullptr;

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
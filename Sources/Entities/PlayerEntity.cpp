#include "Entities/PlayerEntity.hpp"

#include "Core/Debug/Log.hpp"

Entities::PlayerEntity::PlayerEntity()
{
	HitBox = Core::Util::Box(Core::Maths::Vec3(0.0f, 0.95f, 0.0f), Core::Maths::Vec3(0.55f, 1.9f, 0.55f));
	head = model.AddOrReplacePart("head", (new Model::BodyPart())->addBox(-4.0F, -8.0F, -4.0F, 8.0F, 8.0F, 8.0F)->texOffs(0, 0));
	hat = model.AddOrReplacePart("hat", (new Model::BodyPart())->addBox(-4.0F, -8.0F, -4.0F, 8.0F, 8.0F, 8.0F)->extend(0.5f)->texOffs(32, 0));
	body = model.AddOrReplacePart("body", (new Model::BodyPart())->addBox(-4.0F, 0.0F, -2.0F, 8.0F, 12.0F, 4.0F)->texOffs(16, 16));
	rightArm = model.AddOrReplacePart("right_arm", (new Model::BodyPart())->addBox(-3.0F, -2.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(40, 16));
	rightLeg = model.AddOrReplacePart("right_leg", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(0, 16));
	cloak = model.AddOrReplacePart("cloak", (new Model::BodyPart())->addBox(-5.0F, 0.0F, -1.0F, 10.0F, 16.0F, 1.0F)->texOffs(0, 0));
	leftArm = model.AddOrReplacePart("left_arm", (new Model::BodyPart())->addBox(-1.0F, -2.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(32, 48));
	leftSleeve = model.AddOrReplacePart("left_sleeve", (new Model::BodyPart())->addBox(-1.0F, -2.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(48, 48)->extend(0.25F));
	rightSleeve = model.AddOrReplacePart("right_sleeve", (new Model::BodyPart())->addBox(-3.0F, -2.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(40, 32)->extend(0.25F));
	leftLeg = model.AddOrReplacePart("left_leg", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(16, 48));
	leftPants = model.AddOrReplacePart("left_pants", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(0, 48)->extend(0.25F));
	rightPants = model.AddOrReplacePart("right_pants", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, 12.0F, 4.0F)->texOffs(0, 32)->extend(0.25F));
	jacket = model.AddOrReplacePart("jacket", (new Model::BodyPart())->addBox(-4.0F, 0.0F, -2.0F, 8.0F, 12.0F, 4.0F)->texOffs(16, 32)->extend(0.25F));
	rightArm->offset = Core::Maths::Vec3(-5.0F, 2.0F, 0.0F);
	leftArm->offset = Core::Maths::Vec3(5.0F, 2.0F, 0.0F);
	rightSleeve->offset = Core::Maths::Vec3(-5.0F, 2.0F, 0.0F);
	leftSleeve->offset = Core::Maths::Vec3(5.0F, 2.0F, 0.0F);
	rightLeg->offset = Core::Maths::Vec3(-1.9F, 12.0F, 0.0F);
	leftLeg->offset = Core::Maths::Vec3(1.9F, 12.0F, 0.0F);
	leftPants->offset = Core::Maths::Vec3(1.9F, 12.0F, 0.0F);
	rightPants->offset = Core::Maths::Vec3(-1.9F, 12.0F, 0.0F);
    cloak->visible = false;
    model.Bake();
}

void Entities::PlayerEntity::Update(float deltatime)
{
    EyeHeight = crouching ? 1.35f : 1.75f;
    if (attacking)
    {
        attackTime += deltatime * 3;
        if (attackTime > 1.0f)
        {
            attacking = false;
            attackTime = 0.0f;
        }
    }
    if (riding && crouching)
    {
        riding = false;
        ridingEntity = nullptr;
    }
	Entities::EntityLivingBase::Update(deltatime);
}

void Entities::PlayerEntity::DamageEntity(float Amount)
{
	EntityLivingBase::DamageEntity(Amount);
}

void Entities::PlayerEntity::OnDeath()
{
	EntityLivingBase::OnDeath();
}

void Entities::PlayerEntity::Render(World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp, bool IsShadowMap)
{
    Entities::EntityLivingBase::Render(worldIn, shaderProgram, VAOCurrent, vp, IsShadowMap);
}

void Entities::PlayerEntity::SetupAnim(float deltaTime, float globalTime)
{
    Vec3 tmp = Velocity;
    tmp.y = 0.0f;
    float speed = Util::minF(tmp.getLength(), 0.008f) / 0.016f * 2.0f;
    animCounter += speed / 2.0f * deltaTime / 0.016f;
    bool flag = getFallFlyingTicks() > 4;
    bool flag1 = isVisuallySwimming();
    head->rotation.y = (ViewRotation.y - Rotation) * ((float)M_PI / 180.0F);
    if (flag) {
        head->rotation.x = (-(float)M_PI / 4.0F);
    }
    else if (swimAmount > 0.0F) {
        if (flag1) {
            head->rotation.x = rotlerpRad(swimAmount, head->rotation.x, (-(float)M_PI / 4.0F));
        }
        else {
            head->rotation.x = rotlerpRad(swimAmount, head->rotation.x, (ViewRotation.y) * ((float)M_PI / 180.0F));
        }
    }
    else {
        head->rotation.x = (ViewRotation.x) * ((float)M_PI / 180.0F);
    }

    body->rotation.y = 0.0F;
    rightArm->offset.z = 0.0F;
    rightArm->offset.x = -5.0F;
    leftArm->offset.z = 0.0F;
    leftArm->offset.x = 5.0F;
    float f = 1.0F;
    if (flag) {
        f = (float)speed;
        f /= 0.2F;
        f *= f * f;
    }

    if (f < 1.0F) {
        f = 1.0F;
    }

    rightArm->rotation.x = cosf(animCounter * 0.6662F + (float)M_PI) * 2.0F * speed * 0.5F / f;
    leftArm->rotation.x = cosf(animCounter * 0.6662F) * 2.0F * speed * 0.5F / f;
    rightArm->rotation.z = 0.0F;
    leftArm->rotation.z = 0.0F;
    rightLeg->rotation.x = cosf(animCounter * 0.6662F) * 1.4F * speed / f;
    leftLeg->rotation.x = cosf(animCounter * 0.6662F + (float)M_PI) * 1.4F * speed / f;
    rightLeg->rotation.y = 0.0F;
    leftLeg->rotation.y = 0.0F;
    rightLeg->rotation.z = 0.0F;
    leftLeg->rotation.z = 0.0F;
    if (riding) {
        rightArm->rotation.x += (-(float)M_PI / 5.0F);
        leftArm->rotation.x += (-(float)M_PI / 5.0F);
        rightLeg->rotation.x = -1.4137167F;
        rightLeg->rotation.y = ((float)M_PI / 10.0F);
        rightLeg->rotation.z = 0.07853982F;
        leftLeg->rotation.x = -1.4137167F;
        leftLeg->rotation.y = (-(float)M_PI / 10.0F);
        leftLeg->rotation.z = -0.07853982F;
    }

    rightArm->rotation.y = 0.0F;
    leftArm->rotation.y = 0.0F;
    bool flag2 = leftHanded;
    if (isUsingItem()) {
        bool flag3 = true;
        if (flag3 == flag2) {
            poseRightArm();
        }
        else {
            poseLeftArm();
        }
    }
    else {
        bool flag4 = false;
        if (flag2 != flag4) {
            poseLeftArm();
            poseRightArm();
        }
        else {
            poseRightArm();
            poseLeftArm();
        }
    }

    setupAttackAnimation();
    if (crouching) {
        body->rotation.x = 0.5F;
        rightArm->rotation.x += 0.4F;
        leftArm->rotation.x += 0.4F;
        rightLeg->offset.z = 4.0F;
        leftLeg->offset.z = 4.0F;
        rightLeg->offset.y = 12.2F;
        leftLeg->offset.y = 12.2F;
        head->offset.y = 4.2F;
        body->offset.y = 3.2F;
        leftArm->offset.y = 5.2F;
        rightArm->offset.y = 5.2F;
    }
    else {
        body->rotation.x = 0.0F;
        rightLeg->offset.z = 0.1F;
        leftLeg->offset.z = 0.1F;
        rightLeg->offset.y = 12.0F;
        leftLeg->offset.y = 12.0F;
        head->offset.y = 0.0F;
        body->offset.y = 0.0F;
        leftArm->offset.y = 2.0F;
        rightArm->offset.y = 2.0F;
    }
    
    if (rightArmPose != ArmPose::SPYGLASS) {
        bobModelPart(rightArm, globalTime*10, 1.0F);
    }

    if (leftArmPose != ArmPose::SPYGLASS) {
        bobModelPart(leftArm, globalTime*10, -1.0F);
    }
    
    if (swimAmount > 0.0F) {
        float f5 = Util::mod(animCounter, 26.0F);
        float f1 = !leftHanded && attackTime > 0.0F ? 0.0F : swimAmount;
        float f2 = leftHanded && attackTime > 0.0F ? 0.0F : swimAmount;
        if (!isUsingItem()) {
            if (f5 < 14.0F) {
                leftArm->rotation.x = rotlerpRad(f2, leftArm->rotation.x, 0.0F);
                rightArm->rotation.x = Util::lerp(f1, rightArm->rotation.x, 0.0F);
                leftArm->rotation.y = rotlerpRad(f2, leftArm->rotation.y, (float)M_PI);
                rightArm->rotation.y = Util::lerp(f1, rightArm->rotation.y, (float)M_PI);
                leftArm->rotation.z = rotlerpRad(f2, leftArm->rotation.z, (float)M_PI + 1.8707964F * quadraticArmUpdate(f5) / quadraticArmUpdate(14.0F));
                rightArm->rotation.z = Util::lerp(f1, rightArm->rotation.z, (float)M_PI - 1.8707964F * quadraticArmUpdate(f5) / quadraticArmUpdate(14.0F));
            }
            else if (f5 >= 14.0F && f5 < 22.0F) {
                float f6 = (f5 - 14.0F) / 8.0F;
                leftArm->rotation.x = rotlerpRad(f2, leftArm->rotation.x, ((float)M_PI / 2.0F) * f6);
                rightArm->rotation.x = Util::lerp(f1, rightArm->rotation.x, ((float)M_PI / 2.0F) * f6);
                leftArm->rotation.y = rotlerpRad(f2, leftArm->rotation.y, (float)M_PI);
                rightArm->rotation.y = Util::lerp(f1, rightArm->rotation.y, (float)M_PI);
                leftArm->rotation.z = rotlerpRad(f2, leftArm->rotation.z, 5.012389F - 1.8707964F * f6);
                rightArm->rotation.z = Util::lerp(f1, rightArm->rotation.z, 1.2707963F + 1.8707964F * f6);
            }
            else if (f5 >= 22.0F && f5 < 26.0F) {
                float f3 = (f5 - 22.0F) / 4.0F;
                leftArm->rotation.x = rotlerpRad(f2, leftArm->rotation.x, ((float)M_PI / 2.0F) - ((float)M_PI / 2.0F) * f3);
                rightArm->rotation.x = Util::lerp(f1, rightArm->rotation.x, ((float)M_PI / 2.0F) - ((float)M_PI / 2.0F) * f3);
                leftArm->rotation.y = rotlerpRad(f2, leftArm->rotation.y, (float)M_PI);
                rightArm->rotation.y = Util::lerp(f1, rightArm->rotation.y, (float)M_PI);
                leftArm->rotation.z = rotlerpRad(f2, leftArm->rotation.z, (float)M_PI);
                rightArm->rotation.z = Util::lerp(f1, rightArm->rotation.z, (float)M_PI);
            }
        }

        float f7 = 0.3F;
        float f4 = 0.33333334F;
        leftLeg->rotation.x = Util::lerp(swimAmount, leftLeg->rotation.x, 0.3F * cosf(animCounter * 0.33333334F + (float)M_PI));
        rightLeg->rotation.x = Util::lerp(swimAmount, rightLeg->rotation.x, 0.3F * cosf(animCounter * 0.33333334F));
    }

    hat->copyFrom(head);
    leftPants->copyFrom(leftLeg);
    rightPants->copyFrom(rightLeg);
    leftSleeve->copyFrom(leftArm);
    rightSleeve->copyFrom(rightArm);
    jacket->copyFrom(body);
    if (true) {
        if (crouching) {
            cloak->offset.z = 1.4F;
            cloak->offset.y = 1.85F;
        }
        else {
            cloak->offset.z = 0.0F;
            cloak->offset.y = 0.0F;
        }
    }
    else if (crouching) {
        cloak->offset.z = 0.3F;
        cloak->offset.y = 0.8F;
    }
    else {
        cloak->offset.z = -1.1F;
        cloak->offset.y = -0.85F;
    }
}

bool Entities::PlayerEntity::isUsingItem()
{
    return false;
}

float Entities::PlayerEntity::quadraticArmUpdate(float value) {
    return -65.0F * value + value * value;
}

void Entities::PlayerEntity::poseRightArm() {
    switch (rightArmPose) {
    case ArmPose::EMPTY:
        rightArm->rotation.y = 0.0F;
        break;
    case ArmPose::BLOCK:
        rightArm->rotation.x = rightArm->rotation.x * 0.5F - 0.9424779F;
        rightArm->rotation.y = (-(float)M_PI / 6.0F);
        break;
    case ArmPose::ITEM:
        rightArm->rotation.x = rightArm->rotation.x * 0.5F - ((float)M_PI / 10.0F);
        rightArm->rotation.y = 0.0F;
        break;
    case ArmPose::THROW_SPEAR:
        rightArm->rotation.x = rightArm->rotation.x * 0.5F - (float)M_PI;
        rightArm->rotation.y = 0.0F;
        break;
    case ArmPose::BOW_AND_ARROW:
        rightArm->rotation.y = -0.1F + head->rotation.y;
        leftArm->rotation.y = 0.1F + head->rotation.y + 0.4F;
        rightArm->rotation.x = (-(float)M_PI / 2.0F) + head->rotation.x;
        leftArm->rotation.x = (-(float)M_PI / 2.0F) + head->rotation.x;
        break;
    case ArmPose::CROSSBOW_CHARGE:
        //AnimationUtils.animateCrossbowCharge(rightArm, leftArm, p_102876_, true);
        break;
    case ArmPose::CROSSBOW_HOLD:
        //AnimationUtils.animateCrossbowHold(rightArm, leftArm, head, true);
        break;
    case ArmPose::SPYGLASS:
        rightArm->rotation.x = Util::cut(head->rotation.x - 1.9198622F - (crouching ? 0.2617994F : 0.0F), -2.4F, 3.3F);
        rightArm->rotation.y = head->rotation.y - 0.2617994F;
    }

}

void Entities::PlayerEntity::poseLeftArm() {
    switch (leftArmPose) {
    case ArmPose::EMPTY:
        leftArm->rotation.y = 0.0F;
        break;
    case ArmPose::BLOCK:
        leftArm->rotation.x = leftArm->rotation.x * 0.5F - 0.9424779F;
        leftArm->rotation.y = ((float)M_PI / 6.0F);
        break;
    case ArmPose::ITEM:
        leftArm->rotation.x = leftArm->rotation.x * 0.5F - ((float)M_PI / 10.0F);
        leftArm->rotation.y = 0.0F;
        break;
    case ArmPose::THROW_SPEAR:
        leftArm->rotation.x = leftArm->rotation.x * 0.5F - (float)M_PI;
        leftArm->rotation.y = 0.0F;
        break;
    case ArmPose::BOW_AND_ARROW:
        rightArm->rotation.y = -0.1F + head->rotation.y - 0.4F;
        leftArm->rotation.y = 0.1F + head->rotation.y;
        rightArm->rotation.x = (-(float)M_PI / 2.0F) + head->rotation.x;
        leftArm->rotation.x = (-(float)M_PI / 2.0F) + head->rotation.x;
        break;
    case ArmPose::CROSSBOW_CHARGE:
        //AnimationUtils.animateCrossbowCharge(rightArm, leftArm, p_102879_, false);
        break;
    case ArmPose::CROSSBOW_HOLD:
        //AnimationUtils.animateCrossbowHold(rightArm, leftArm, head, false);
        break;
    case ArmPose::SPYGLASS:
        leftArm->rotation.x = Util::cut(head->rotation.x - 1.9198622F - (crouching ? 0.2617994F : 0.0F), -2.4F, 3.3F);
        leftArm->rotation.y = head->rotation.y + 0.2617994F;
    }

}

void Entities::PlayerEntity::setupAttackAnimation() {
    if (!(attackTime <= 0.0F)) {
        Model::BodyPart* modelpart = leftHanded ? leftArm : rightArm;
        float f = attackTime;
        body->rotation.y = sinf(sqrtf(f) * ((float)M_PI * 2.0F)) * 0.2F;
        if (leftHanded) {
            body->rotation.y *= -1.0F;
        }

        rightArm->offset.z = sinf(body->rotation.y) * 5.0F;
        rightArm->offset.x = -cosf(body->rotation.y) * 5.0F;
        leftArm->offset.z = -sinf(body->rotation.y) * 5.0F;
        leftArm->offset.x = cosf(body->rotation.y) * 5.0F;
        rightArm->rotation.y += body->rotation.y;
        leftArm->rotation.y += body->rotation.y;
        leftArm->rotation.x += body->rotation.y;
        f = 1.0F - attackTime;
        f *= f;
        f *= f;
        f = 1.0F - f;
        float f1 = sinf(f * (float)M_PI);
        float f2 = sinf(attackTime * (float)M_PI) * -(head->rotation.x - 0.7F) * 0.75F;
        modelpart->rotation.x -= f1 * 1.2F + f2;
        modelpart->rotation.y += body->rotation.y * 2.0F;
        modelpart->rotation.z += sinf(attackTime * (float)M_PI) * -0.4F;
    }
}

void Entities::PlayerEntity::bobModelPart(Model::BodyPart* part, float globalTime, float factor) {
    part->rotation.z += factor * (cosf(globalTime * 0.09F) * 0.05F + 0.05F);
    part->rotation.x += factor * sinf(globalTime * 0.067F) * 0.05F;
}
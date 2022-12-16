#include "Entities/PigEntity.hpp"

#include "Core/Debug/Log.hpp"
#include "Core/App/App.hpp"

Entities::PigEntity::PigEntity()
{
    uuid = Core::App::App::GetRNG().nextLong();
    model.SetTexture(manager->Get<Resources::Texture>("Resources/textures/entity/pig/pig.png"));
    OnGround = true;
    MaxHealth = 10.0f;
    Health = 10.0f;
    MovementSpeed = 0.0025f;
    Rotation = 0.0f;
    EyeHeight = 0.6f;
    HitBox = Core::Util::Box(Core::Maths::Vec3(0.0f, 0.45f, 0.0f), Core::Maths::Vec3(0.8f, 0.9f, 0.8f));

    float height = 6.0f;

    head = model.AddOrReplacePart("head", (new Model::BodyPart())->addBox(-4.0F, -4.0F, 0.0F, 8.0F, 8.0F, 8.0F)->texOffs(0, 0)->addBox(-2.0F, 0.0F, 8.0F, 4.0F, 3.0F, 1.0F)->texOffs(16,16));
    body = model.AddOrReplacePart("body", (new Model::BodyPart())->addBox(-5.0F, -10.0F, -7.0F, 10.0F, 16.0F, 8.0F)->texOffs(28, 8));
    rightHindLeg = model.AddOrReplacePart("right_hind_leg", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, height, 4.0F)->texOffs(0, 16));
    leftHindLeg = model.AddOrReplacePart("left_hind_leg", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, height, 4.0F)->texOffs(0, 16));
    rightFrontLeg = model.AddOrReplacePart("right_front_leg", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, height, 4.0F)->texOffs(0, 16));
    leftFrontLeg = model.AddOrReplacePart("left_front_leg", (new Model::BodyPart())->addBox(-2.0F, 0.0F, -2.0F, 4.0F, height, 4.0F)->texOffs(0, 16));

    head->offset = Core::Maths::Vec3(0.0F, 12.0F, -6.0F);
    body->offset = Core::Maths::Vec3(0.0F, 17.0f, 2.0F);
    body->rotation = Core::Maths::Vec3((float)M_PI_2, 0.0F, 0.0F);
    rightHindLeg->offset = Core::Maths::Vec3(-3.0F, 24.0F - height, 7.0F);
    leftHindLeg->offset = Core::Maths::Vec3(3.0F, 24.0F - height, 7.0F);
    rightFrontLeg->offset = Core::Maths::Vec3(-3.0F, 24.0F - height, -5.0F);
    leftFrontLeg->offset = Core::Maths::Vec3(3.0F, 24.0F - height, -5.0F);

    model.Bake();
}

void Entities::PigEntity::Update(float deltatime)
{
    Entities::EntityLivingBase::Update(deltatime);
}

void Entities::PigEntity::SetupAnim(float deltaTime, float globalTime)
{
    Vec3 tmp = Velocity;
    tmp.y = 0.0f;
    float speed = Util::minF(tmp.getLength(), 0.008f) / 0.016f * 2.0f;
    animCounter += speed / 2.0f * deltaTime / 0.016f;

    head->rotation.x = (ViewRotation.x) * ((float)M_PI / 180.0F);
    head->rotation.y = (ViewRotation.y - Rotation) * ((float)M_PI / 180.0F);
    rightHindLeg->rotation.x = cosf(animCounter * 0.6662F) * 1.4F * speed;
    leftHindLeg->rotation.x = cosf(animCounter * 0.6662F + (float)M_PI) * 1.4F * speed;
    rightFrontLeg->rotation.x = cosf(animCounter * 0.6662F + (float)M_PI) * 1.4F * speed;
    leftFrontLeg->rotation.x = cosf(animCounter * 0.6662F) * 1.4F * speed;
}

Vec3 Entities::PigEntity::getRidePosition(EntityLivingBase* entity)
{
    return Position + Vec3(0.0f,0.3f,0.0f);
}

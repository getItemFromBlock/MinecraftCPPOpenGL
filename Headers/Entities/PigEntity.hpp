#pragma once

#include "EntityLivingBase.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Model/ArmPose.hpp"

namespace Entities
{
    class PigEntity : public EntityLivingBase
    {
    public:

        PigEntity();

        virtual void Update(float deltatime) override;
        virtual void SetupAnim(float deltaTime, float globalTime) override;
    protected:
        Model::BodyPart* head = nullptr;
        Model::BodyPart* body = nullptr;
        Model::BodyPart* rightHindLeg = nullptr;
        Model::BodyPart* leftHindLeg = nullptr;
        Model::BodyPart* rightFrontLeg = nullptr;
        Model::BodyPart* leftFrontLeg = nullptr;

        virtual Vec3 getRidePosition(EntityLivingBase* entity) override;
    };
}

#include "Entities/ClientPlayerEntity.hpp"

#include <ImGUI/imgui.h>

#include "Core/App/App.hpp"
#include "Core/Debug/Gizmo.hpp"
#include "Entities/PigEntity.hpp"

Entities::ClientPlayerEntity::ClientPlayerEntity(const char* name, bool isSlim) : PlayerEntity(name, isSlim)
{
	Bindings = Core::App::App::GetInputBindings();
	camera = Core::App::App::GetMainCamera();
	inputs = Core::App::App::GetInputs();
}

void Entities::ClientPlayerEntity::Update(float deltatime)
{
	if (ImGui::IsKeyPressed(Bindings[static_cast<unsigned int>(Core::App::InputType::View)]))
	{
		viewMode = GetNext();
	}
	MovementOut = Vec3();
	for (unsigned int i = 0; i < 4; i++)
	{
		unsigned int index = (i < 2) ? 2 : 0;
		if (ImGui::IsKeyDown(Bindings[i])) MovementOut[index] += (i % 2 == 0 ? 1.0f : -1.0f);
	}
	if (MovementOut.getLength() < 0.1f) MovementOut = Core::Maths::Vec3();
	else if (MovementOut.getLength() > 1.0f) MovementOut = MovementOut.unitVector();
	MovementOut = (Core::Maths::Mat4::CreateYRotationMatrix(ViewRotation.y) * MovementOut).getVector();
	if (ImGui::IsKeyPressed(Bindings[static_cast<unsigned int>(Core::App::InputType::Jump)])) Jump();
	crouching = ImGui::IsKeyDown(Bindings[static_cast<unsigned int>(Core::App::InputType::Crouch)]);
	effectiveSpeed = 1.0f;
	if (crouching)
	{
		effectiveSpeed = 0.4f;
	}
	else if (ImGui::IsKeyDown(Bindings[static_cast<unsigned int>(Core::App::InputType::Run)]))
	{
		effectiveSpeed = 1.6f;
	}
	Entities::PlayerEntity::Update(deltatime);
}

void Entities::ClientPlayerEntity::DamageEntity(float Amount)
{
	PlayerEntity::DamageEntity(Amount);
}

void Entities::ClientPlayerEntity::OnDeath()
{
	PlayerEntity::OnDeath();
}

void Entities::ClientPlayerEntity::Render(World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp, bool IsShadowMap)
{
	if (IsShadowMap ||viewMode != CameraViewMode::DEFAULT) PlayerEntity::Render(worldIn, shaderProgram, VAOCurrent, vp, IsShadowMap);
}

void Entities::ClientPlayerEntity::Jump()
{
	if (riding && ridingEntity && ridingEntity->OnGround)
	{
		Entities::PigEntity* pig = dynamic_cast<Entities::PigEntity*>(ridingEntity);
		if (pig)
		{
			pig->Jump();
		}
	}
	else
	{
		EntityLivingBase::Jump();
	}
}

void Entities::ClientPlayerEntity::ClientUpdate(float deltatime, World::World* world)
{
	camera->Update(*inputs, Position + Core::Maths::Vec3(0.0f, EyeHeight, 0.0f), viewMode, deltatime);
	ViewRotation = Core::Maths::Vec3(camera->rotation.y, camera->rotation.x + 180, camera->rotation.z);
	Rotation = ViewRotation.y;
	Core::Maths::Vec3 lookAt = (Core::Maths::Mat4::CreateRotationMatrix(ViewRotation) * Core::Maths::Vec3(0, 0, 5)).getVector();
	lookAt = lookAt + Position + Core::Maths::Vec3(0.0f, EyeHeight, 0.0f);
	Core::Debug::OverlayGizmo::PushElement(Core::Util::Box(lookAt + Core::Maths::Vec3(0.1f, 0.0f, 0.0f), Core::Maths::Vec3(0.2f, 0.01f, 0.01f)), Core::Maths::Vec3(1, 0, 0));
	Core::Debug::OverlayGizmo::PushElement(Core::Util::Box(lookAt + Core::Maths::Vec3(0.0f, 0.1f, 0.0f), Core::Maths::Vec3(0.01f, 0.2f, 0.01f)), Core::Maths::Vec3(0, 1, 0));
	Core::Debug::OverlayGizmo::PushElement(Core::Util::Box(lookAt + Core::Maths::Vec3(0.0f, 0.0f, 0.1f), Core::Maths::Vec3(0.01f, 0.01f, 0.2f)), Core::Maths::Vec3(0, 0, 1));
	if (inputs->scroll > 0.1f) selectedBlock = Blocks::BlockRegister::getNext(selectedBlock);
	else if (inputs->scroll < -0.1f) selectedBlock = Blocks::BlockRegister::getPrevious(selectedBlock);
	Physics::EntityRayCastResult eResult = Physics::PhysicsHandler::RayCastEntity(world, Position + Core::Maths::Vec3(0.0f, EyeHeight, 0.0f), lookAt, this);
	Physics::BlockRayCastResult bResult = Physics::PhysicsHandler::RayCastBlock(world, Position + Core::Maths::Vec3(0.0f, EyeHeight, 0.0f), lookAt);
	if (inputs->mouseCaptured)
	{
		if (inputs->leftMouse & INPUT_PRESS)
		{
			attacking = true;
			attackTime = 0.0f;
		}
		if (bResult.hit && (!eResult.hit || bResult.distance <= eResult.distance))
			{
				Vec3 center = bResult.point + Vec3(bResult.normal) * 0.25f;
				Vec3 size = Vec3(fabsf(bResult.normal.x / 2.0f), fabsf(bResult.normal.y / 2.0f), fabsf(bResult.normal.z / 2.0f)) + 0.01f;
				Core::Debug::OverlayGizmo::PushElement(Core::Util::Box(center, size));
				if (inputs->leftMouse & INPUT_PRESS)
				{
					world->SetBlockAt(bResult.position, Blocks::BlockRegister::GetBlock(BLOCK::AIR));
				}
				else if (inputs->rightMouse & INPUT_PRESS)
				{
					Blocks::Block* block = world->GetBlockAt(bResult.position + bResult.normal);
					if (block && block->GetCollisionType() == Core::Util::CollisionType::NONE)
					{
						world->SetBlockAt(bResult.position + bResult.normal, Blocks::BlockRegister::GetBlock(selectedBlock));
						attacking = true;
						attackTime = 0.0f;
					}
				}
			}
		else if (eResult.hit)
		{
			Vec3 center = eResult.point + Vec3(eResult.normal) * 0.25f;
			Vec3 size = Vec3(fabsf(eResult.normal.x / 2.0f), fabsf(eResult.normal.y / 2.0f), fabsf(eResult.normal.z / 2.0f)) + 0.01f;
			Core::Debug::OverlayGizmo::PushElement(Core::Util::Box(center, size));
			if (inputs->leftMouse & INPUT_PRESS)
			{
				riding = true;
				ridingEntity = eResult.target;
				//eResult.target->Jump();
			}
		}
	}
	if (riding)
	{
		if (crouching)
		{
			riding = false;
			ridingEntity = nullptr;
		}
		else if (ridingEntity)
		{
			Entities::PigEntity* pig = dynamic_cast<Entities::PigEntity*>(ridingEntity);
			if (pig)
			{
				pig->MovementOut = MovementOut;
				pig->Rotation = Rotation;
				pig->ViewRotation.y = ViewRotation.y;
			}
		}
	}
}

BLOCK Entities::ClientPlayerEntity::GetSelectedBlock()
{
	return selectedBlock;
}

CameraViewMode Entities::ClientPlayerEntity::GetNext()
{
	char v = static_cast<char>(viewMode) + 1;
	if (v > static_cast<char>(CameraViewMode::FRONT)) v = 0;
	return CameraViewMode(v);
}

#include "Entities/PlayerEntity.hpp"

#include <ImGUI/imgui.h>

#include "Core/App/App.hpp"

Entities::PlayerEntity::PlayerEntity()
{
	Bindings = Core::App::App::GetInputBindings();
	HitBox = Core::Util::Box(Core::Maths::Vec3D(0.0f, 0.95f, 0.0f), Core::Maths::Vec3D(0.55f, 1.9f, 0.55f));
}

void Entities::PlayerEntity::Update(float deltatime)
{
	Core::Maths::Vec3D VOut;
	for (unsigned int i = 0; i < 4; i++)
	{
		unsigned int index = (i < 2) ? 2 : 0;
		if (ImGui::IsKeyDown(Bindings[i])) VOut[index] += (i % 2 == 0 ? 1.0f : -1.0f);
	}
	if (VOut.getLength() < 0.1f) VOut = Core::Maths::Vec3D();
	else if (VOut.getLength() > 1.0f) VOut = VOut.unitVector();
	VOut = (Core::Maths::Mat4D::CreateYRotationMatrix(ViewRotation.y) * VOut).getVector();
	if (OnGround)
	{
		Velocity.y = 0;
		if (ImGui::IsKeyPressed(Bindings[4]))
		{
			Velocity.y += 0.008f;
			OnGround = false;
		}
	}
	float tmp = Velocity.y;
	Velocity = Velocity + VOut * deltatime * (OnGround ? MovementSpeed * 18.0f : MovementSpeed * 1.0f);
	Velocity = Velocity * (OnGround ? 1.0f - 10.0f * deltatime : 1.0f - 1.0f * deltatime);
	Velocity.y = tmp;

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

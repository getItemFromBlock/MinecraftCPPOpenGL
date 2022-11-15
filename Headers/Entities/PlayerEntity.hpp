#pragma once

#include "EntityLivingBase.hpp"

#include "Core/Util/HitBox.hpp"
#include "Core/Util/Shape.hpp"

#define GRAVITY 0.01f

namespace Entities
{
	class PlayerEntity : public EntityLivingBase
	{
	public:

		PlayerEntity();

		virtual void Update(float deltatime) override;
		virtual void DamageEntity(float Amount) override;
		virtual void OnDeath() override;
	protected:
		int* Bindings = nullptr;
	};
}
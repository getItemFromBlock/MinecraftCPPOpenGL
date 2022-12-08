#pragma once

#include "PlayerEntity.hpp"

#include "Core/Util/HitBox.hpp"
#include "Core/Util/Shape.hpp"
#include "Blocks/Blocks.hpp"
#include "LowRenderer/Rendering/EditorCamera.hpp"

namespace Core
{
	namespace App
	{
		class Inputs;
	}
}

namespace Entities
{
	class ClientPlayerEntity : public PlayerEntity
	{
	public:

		ClientPlayerEntity();

		virtual void Update(float deltatime) override;
		virtual void DamageEntity(float Amount) override;
		virtual void OnDeath() override;
		virtual void Render(World::World* worldIn, Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4& vp, bool IsShadowMap) override;
		void ClientUpdate(float deltatime, World::World* world);
		BLOCK GetSelectedGlock();
	protected:
		int* Bindings = nullptr;
		LowRenderer::Rendering::EditorCamera* camera = nullptr;
		Core::App::Inputs* inputs = nullptr;
		BLOCK selectedBlock = BLOCK::COBBLESTONE;
		CameraViewMode viewMode = CameraViewMode::DEFAULT;

		CameraViewMode GetNext();
	};
}
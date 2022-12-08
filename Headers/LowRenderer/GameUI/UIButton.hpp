#pragma once

#include <string>

#include "LowRenderer/GameUI/UISelectable.hpp"

namespace Resources
{
	class Mesh;
}

namespace LowRenderer::GameUI
{
	class UIButton : public UISelectable
	{
	public:
		UIButton();
		~UIButton();

		virtual void DeleteElement() override;
		virtual void RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& v, const Core::Maths::Vec2 ScreenRes, const Core::Maths::Vec2 MousePos, float ScrollValue, unsigned int Inputs) override;
		virtual void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime) override;
		virtual void OnClick() override;
	protected:
		Core::Maths::Vec3 BaseColor = Core::Maths::Vec3(0.6f);
		Core::Maths::Vec3 HoverColor = Core::Maths::Vec3(0.8f);
		Core::Maths::Vec3 ClickColor = Core::Maths::Vec3(0.6f, 1.0f, 0.6f);
	};

}
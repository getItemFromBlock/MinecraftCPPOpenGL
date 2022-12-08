#pragma once

#include "LowRenderer/GameUI/UIButton.hpp"

namespace Core::App
{
	enum class InputType : unsigned int;
}

namespace LowRenderer::GameUI
{
	class UIBindingButton : public UIButton
	{
	public:
		UIBindingButton();
		~UIBindingButton();

		virtual void DeleteElement() override;
		virtual void RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& v, const Core::Maths::Vec2 ScreenRes, const Core::Maths::Vec2 MousePos, float ScrollValue, unsigned int Inputs) override;
		virtual void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime) override;
		virtual void OnClick() override;
	private:
		void UpdateKeyText();

		Core::App::InputType Type = static_cast<Core::App::InputType>(0);
		float Delay = 3.0f;
		float Counter = -1.0f;
		Core::Maths::Vec3 WaitColor = Core::Maths::Vec3(0.4f, 0.4f, 1.0f);
		Core::Maths::Vec3 TmpColor[3] = {0};
	};

}

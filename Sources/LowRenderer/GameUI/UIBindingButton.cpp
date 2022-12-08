#include "LowRenderer/GameUI/UIBindingButton.hpp"

#include <ImGUI/imgui.h>

#include "Core/Util/TextHelper.hpp"
#include "Core/App/App.hpp"
#include "LowRenderer/Rendering/RenderCamera.hpp"

LowRenderer::GameUI::UIBindingButton::UIBindingButton()
{
}

LowRenderer::GameUI::UIBindingButton::~UIBindingButton()
{
}

void LowRenderer::GameUI::UIBindingButton::DeleteElement()
{
	UIButton::DeleteElement();
	this->~UIBindingButton();
}

void LowRenderer::GameUI::UIBindingButton::RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& v, const Core::Maths::Vec2 ScreenRes, const Core::Maths::Vec2 MousePos, float ScrollValue, unsigned int Inputs)
{
	if (!Plane) return;
	if (Label == "Text") UpdateKeyText();
	UIButton::RenderGameUI(VAOCurrent, shaderProgramCurrent, v, ScreenRes, MousePos, ScrollValue, Inputs);
}

void LowRenderer::GameUI::UIBindingButton::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime)
{
	UIButton::Update(cameras, resources, textureManager, lightManager, DeltaTime);
	if (Counter >= 0.0f)
	{
		int key = Core::App::App::GetLastKeyPressed();
		if (key)
		{
			Core::App::App::GetInputBindings()[static_cast<unsigned int>(Type)] = key;
			Counter = Delay;
		}
		if (Counter >= Delay)
		{
			BaseColor = TmpColor[0];
			HoverColor = TmpColor[1];
			ClickColor = TmpColor[2];
			Counter = -1.0f;
			UpdateKeyText();
			return;
		}
		Counter += DeltaTime;
	}
}

void LowRenderer::GameUI::UIBindingButton::OnClick()
{
	if (Counter >= 0.0f) return;
	TmpColor[0] = BaseColor;
	TmpColor[1] = HoverColor;
	TmpColor[2] = ClickColor;
	BaseColor = WaitColor;
	HoverColor = WaitColor;
	ClickColor = WaitColor;
	Core::App::App::ClearLastKeyPressed();
	Counter = 0.0f;
	Label.clear();
	Label = "...";
	UIText::UpdateText();
}

void LowRenderer::GameUI::UIBindingButton::UpdateKeyText()
{
	Label.clear();
	Label = ImGui::GetKeyName(Core::App::App::GetInputBindings()[static_cast<unsigned int>(Type)]);
	UIText::UpdateText();
}

#include "LowRenderer/GameUI/UIExitButton.hpp"

#include "Core/App/App.hpp"
#include "Core/Util/TextHelper.hpp"

LowRenderer::GameUI::UIExitButton::UIExitButton()
{
}

LowRenderer::GameUI::UIExitButton::~UIExitButton()
{
}

void LowRenderer::GameUI::UIExitButton::DeleteElement()
{
	UIButton::DeleteElement();
	this->~UIExitButton();
}

void LowRenderer::GameUI::UIExitButton::OnClick()
{
	Core::App::App::RequestExit();
}

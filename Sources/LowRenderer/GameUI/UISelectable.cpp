#include "LowRenderer/GameUI/UISelectable.hpp"

#include <ImGUI/imgui.h>

#include "Core/Util/TextHelper.hpp"
#include "Core/App/App.hpp"

const char* const DirectionNames[4] =
{
	"Up Component ID",
	"Down Component ID",
	"Left Component ID",
	"Right Component ID",
};

LowRenderer::GameUI::UISelectable::UISelectable()
{
}

LowRenderer::GameUI::UISelectable::~UISelectable()
{
}

void LowRenderer::GameUI::UISelectable::DeleteElement()
{
	UIText::DeleteElement();
	this->~UISelectable();
}

void LowRenderer::GameUI::UISelectable::RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& v, const Core::Maths::Vec2 ScreenRes, const Core::Maths::Vec2 MousePos, float ScrollValue, unsigned int Inputs)
{
	UIText::RenderGameUI(VAOCurrent, shaderProgramCurrent, v, ScreenRes, MousePos, ScrollValue, Inputs);
	if (!ID) return;
	if (ID == Core::App::App::GetSelectedComponent())
	{
		Hovered = true;
		if (Inputs & 0x100000)
		{
			Clicked = true;
			OnClick();
		}
		if (Inputs & 0xF0000)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				if (NavIDs[i] && (Inputs & 1 << (i + 16)))
				{
					Core::App::App::SetSelectedComponent(NavIDs[i]);
				}
			}
		}
	}
}

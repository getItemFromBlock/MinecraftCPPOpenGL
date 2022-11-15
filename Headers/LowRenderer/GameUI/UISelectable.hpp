#pragma once

#pragma once

#include <string>

#include "LowRenderer/GameUI/UIText.hpp"

namespace LowRenderer::GameUI
{
	class UISelectable : public UIText
	{
	public:
		UISelectable();
		~UISelectable();

		virtual void DeleteElement() override;
		virtual void RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4D& v, const Core::Maths::Vec2D ScreenRes, const Core::Maths::Vec2D MousePos, float ScrollValue, unsigned int Inputs) override;
	protected:
		unsigned int ID = 0;
		unsigned int NavIDs[4] = { 0 };
	};

}
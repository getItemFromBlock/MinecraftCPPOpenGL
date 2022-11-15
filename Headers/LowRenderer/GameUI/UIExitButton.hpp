#pragma once

#include "LowRenderer/GameUI/UIButton.hpp"

namespace LowRenderer::GameUI
{
	class UIExitButton : public UIButton
	{
	public:
		UIExitButton();
		~UIExitButton();

		virtual void DeleteElement() override;
		virtual void OnClick() override;
	protected:
	};

}
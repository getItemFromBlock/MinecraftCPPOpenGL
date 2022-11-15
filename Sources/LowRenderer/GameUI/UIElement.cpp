#include "LowRenderer/GameUI/UIElement.hpp"

#include <ImGUI/imgui.h>

#include "Core/Util/TextHelper.hpp"
#include "Core/App/App.hpp"

LowRenderer::GameUI::UIElement::UIElement()
{
}

LowRenderer::GameUI::UIElement::~UIElement()
{
}

void LowRenderer::GameUI::UIElement::DeleteElement()
{
	this->~UIElement();
}

inline void LowRenderer::GameUI::UIElement::PreUpdate()
{
	if (Clicked && !Hovered)
	{
		Clicked = false;
	}
}

void LowRenderer::GameUI::UIElement::RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4D& v, const Core::Maths::Vec2D ScreenRes, const Core::Maths::Vec2D MousePos, float ScrollValue, unsigned int MouseInputs)
{
	Core::Maths::Vec2D MinC;
	float Ratio = ScreenRes.x / ScreenRes.y;
	Core::Maths::Vec2D Sz = ElementSize * Core::Maths::Vec2D(AdaptToScreen ? ScreenRes.x : ScreenRes.y, ScreenRes.y) * 0.5f;
	MinC.x = (ElementPos.x + (CompareAnchor(AnchorType::Left) ? 0 : (CompareAnchor(AnchorType::Right) ? 2 : 1))) * ScreenRes.x * 0.5f - Sz.x;
	MinC.y = (ElementPos.y + (CompareAnchor(AnchorType::Up) ? 0 : (CompareAnchor(AnchorType::Down) ? 2 : 1))) * ScreenRes.y * 0.5f - Sz.y;
	Core::Maths::Vec2D MaxC;
	MaxC.x = (ElementPos.x + (CompareAnchor(AnchorType::Left) ? 0 : (CompareAnchor(AnchorType::Right) ? 2 : 1))) * ScreenRes.x * 0.5f + Sz.x;
	MaxC.y = (ElementPos.y + (CompareAnchor(AnchorType::Up) ? 0 : (CompareAnchor(AnchorType::Down) ? 2 : 1))) * ScreenRes.y * 0.5f + Sz.y;
	ElementMat = Core::Maths::Mat4D::CreateTransformMatrix(Core::Maths::Vec3D(((MinC.x + Sz.x) / ScreenRes.x * 2 - 1) * Ratio, (1 - (MinC.y + Sz.y) / ScreenRes.y) * 2 - 1, ElementPos.z * 2 - 1), Core::Maths::Vec3D(0, 0, ElementRot), Core::Maths::Vec3D(Sz.x / ScreenRes.y * 2.0f, Sz.y / ScreenRes.y * 2.0f, 1));
	Hovered = MousePos.x >= MinC.x && MousePos.x <= MaxC.x && MousePos.y >= MinC.y && MousePos.y <= MaxC.y;
	if (Clicked && !(MouseInputs & static_cast<unsigned int>(Core::App::MouseInput::ALL_DOWN))) Clicked = false;
	if (!Clicked && Hovered && (MouseInputs & static_cast<unsigned int>(Core::App::MouseInput::ALL_PRESS)))
	{
		Clicked = true;
		OnClick();
	}
}

bool LowRenderer::GameUI::UIElement::CompareAnchor(AnchorType other)
{
	return (static_cast<unsigned int>(Anchor) & static_cast<unsigned int>(other));
}

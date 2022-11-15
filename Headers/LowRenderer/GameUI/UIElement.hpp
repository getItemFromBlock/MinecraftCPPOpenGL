#pragma once

#include "Core/Maths/Maths.hpp"

namespace Resources
{
	class ShaderProgram;
	class TextureManager;
}

namespace LowRenderer
{
	namespace Lightning
	{
		class LightManager;
	}

	namespace GameUI
	{
			enum class AnchorType : unsigned int
			{
				Center = 0,
				Up = 1,
				Down = 2,
				Right = 4,
				UpRight = 5,
				DownRight = 6,
				Left = 8,
				UpLeft = 9,
				DownLeft = 10,
			};

			class UIElement
			{
			public:
				UIElement();
				~UIElement();

				virtual void DeleteElement();
				virtual void PreUpdate();
				virtual void RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4D& v, const Core::Maths::Vec2D ScreenRes, const Core::Maths::Vec2D MousePos, float ScrollValue, unsigned int Inputs);
				virtual void OnClick() = 0;
				bool CompareAnchor(AnchorType other);
			protected:
				Core::Maths::Vec3D ElementPos = Core::Maths::Vec3D(0);
				Core::Maths::Vec2D ElementSize = Core::Maths::Vec2D(0.2f, 0.2f);
				Core::Maths::Mat4D ElementMat = Core::Maths::Mat4D::Identity();
				float Alpha = 1.0f;
				float ElementRot = 0;
				bool AdaptToScreen = false;
				AnchorType Anchor = AnchorType::Center;
				bool Clicked = false;
				bool Hovered = false;
			};
		}
	}
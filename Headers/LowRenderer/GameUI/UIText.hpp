#pragma once

#include <string>

#include "LowRenderer/GameUI/UIElement.hpp"
#include "LowRenderer/Rendering/RenderCamera.hpp"

namespace Resources
{
	class Mesh;
	class Font;
}

namespace LowRenderer::GameUI
{
	struct LineData
	{
		float TotalLength;
		std::string line;
	};

	class UIText : public UIElement
	{
	public:
		UIText();
		~UIText();

		virtual void DeleteElement() override;
		virtual void RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4D& v, const Core::Maths::Vec2D ScreenRes, const Core::Maths::Vec2D MousePos, float ScrollValue, unsigned int Inputs) override;
		virtual void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime);
		virtual void OnClick() override;
	protected:
		std::string Label = "Text";
		Resources::Mesh* Plane = nullptr;
		Resources::Font* MFont = nullptr;
		float TextSize = 4.0f;
		Core::Maths::Vec3D TextColor = Core::Maths::Vec3D(0.0f, 0.0f, 0.0f);
		Core::Maths::Mat4D TextMat = Core::Maths::Mat4D::Identity();
		std::vector<LineData> lines;

		void UpdateText();
	};

}
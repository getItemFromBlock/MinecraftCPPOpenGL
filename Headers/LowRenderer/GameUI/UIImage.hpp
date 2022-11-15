#pragma once

#include "LowRenderer/GameUI/UIElement.hpp"
#include "Resources/Material.hpp"
#include "LowRenderer/Rendering/RenderCamera.hpp"

namespace Resources
{
	class Mesh;
}

namespace LowRenderer::GameUI
{
	class UIImage : public UIElement
	{
	public:
		UIImage();
		~UIImage();

		virtual void DeleteElement() override;
		virtual void RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4D& v, const Core::Maths::Vec2D ScreenRes, const Core::Maths::Vec2D MousePos, float ScrollValue, unsigned int Inputs) override;
		virtual void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime);
		virtual void OnClick() override;
	private:
		Core::Maths::Vec3D BaseColor = Core::Maths::Vec3D(0.6f);
		Resources::Mesh* Plane = nullptr;
		Resources::Material Mat;
		std::string tmpTexturePath = "";
		float DeltaF = 0.0f;
	};

}
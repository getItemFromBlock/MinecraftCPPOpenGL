#include "LowRenderer/GameUI/UIButton.hpp"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_stdlib.hpp>
#include <glad/glad.h>

#include "Core/Debug/Log.hpp"

#include "Core/Util/TextHelper.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/ResourceManager.hpp"
#include "Core/App/App.hpp"
#include "LowRenderer/Rendering/RenderCamera.hpp"

LowRenderer::GameUI::UIButton::UIButton()
{
}

LowRenderer::GameUI::UIButton::~UIButton()
{
}

void LowRenderer::GameUI::UIButton::DeleteElement()
{
	UISelectable::DeleteElement();
	this->~UIButton();
}

void LowRenderer::GameUI::UIButton::RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4D& v, const Core::Maths::Vec2D ScreenRes, const Core::Maths::Vec2D MousePos, float ScrollValue, unsigned int MouseInputs)
{
	if (!Plane) return;
	Core::Maths::Mat4D mv = v * ElementMat;
	glUniform1i((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::Texture), Core::App::DefaultTextures::Blank);
	glUniformMatrix4fv((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MVP), 1, GL_FALSE, mv.content);
	if (Clicked) glUniform3f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAmbient), ClickColor.x, ClickColor.y, ClickColor.z);
	else if (Hovered) glUniform3f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAmbient), HoverColor.x, HoverColor.y, HoverColor.z);
	else glUniform3f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAmbient), BaseColor.x, BaseColor.y, BaseColor.z);
	glUniform1f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAlpha), Alpha);
	Plane->Draw(VAOCurrent);
	UISelectable::RenderGameUI(VAOCurrent, shaderProgramCurrent, v, ScreenRes, MousePos, ScrollValue, MouseInputs);
}

void LowRenderer::GameUI::UIButton::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime)
{
	UISelectable::Update(cameras, resources, textureManager, lightManager, DeltaTime);
}

void LowRenderer::GameUI::UIButton::OnClick()
{
}

#include "LowRenderer/GameUI/UIImage.hpp"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_stdlib.hpp>
#include <glad/glad.h>

#include "Core/Debug/Log.hpp"

#include "Core/Util/TextHelper.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/ResourceManager.hpp"
#include "Core/App/App.hpp"
#include "LowRenderer/Rendering/RenderCamera.hpp"

LowRenderer::GameUI::UIImage::UIImage()
{
}

LowRenderer::GameUI::UIImage::~UIImage()
{
}

void LowRenderer::GameUI::UIImage::DeleteElement()
{
	Mat.UnLoad();
	UIElement::DeleteElement();
	this->~UIImage();
}

void LowRenderer::GameUI::UIImage::RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& v, const Core::Maths::Vec2 ScreenRes, const Core::Maths::Vec2 MousePos, float ScrollValue, unsigned int MouseInputs)
{
	UIElement::RenderGameUI(VAOCurrent, shaderProgramCurrent, v, ScreenRes, MousePos, ScrollValue, MouseInputs);
	if (!Plane) return;
	Core::Maths::Mat4 mv = v * ElementMat;
	if (Mat.GetTexture()) glUniform1i((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::Texture), Mat.GetTexture()->GetTextureID());
	else glUniform1i((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::Texture), Core::App::DefaultTextures::Blank);
	glUniformMatrix4fv((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MVP), 1, GL_FALSE, mv.content);
	glUniform3f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAmbient), BaseColor.x, BaseColor.y, BaseColor.z);
	glUniform1f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAlpha), Alpha);
	Plane->Draw(VAOCurrent);
}

void LowRenderer::GameUI::UIImage::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime)
{
	if (!Plane) Plane = resources->Get<Resources::Mesh>("DebugPlane");
	if (tmpTexturePath.c_str()[0])
	{
		DeltaF++;
		Resources::Texture* tmp = resources->Get<Resources::Texture>(tmpTexturePath.c_str());
		if (tmp)
		{
			Mat.SetTexture(tmp);
			tmpTexturePath = "";
		}
		else
		{
			if (DeltaF > 8)
			{
				LOG("Warning, could not find texture %s", tmpTexturePath.c_str());
				tmpTexturePath = "";
			}
		}
	}
}

void LowRenderer::GameUI::UIImage::OnClick()
{
}

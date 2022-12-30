#include "LowRenderer/GameUI/UIText.hpp"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_stdlib.hpp>
#include <glad/glad.h>

#include "Core/Debug/Log.hpp"

#include "Core/Util/TextHelper.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/ResourceManager.hpp"
#include "Core/App/App.hpp"
#include "Resources/Font.hpp"

LowRenderer::GameUI::UIText::UIText()
{
}

LowRenderer::GameUI::UIText::~UIText()
{
}

void LowRenderer::GameUI::UIText::DeleteElement()
{
	UIElement::DeleteElement();
	this->~UIText();
}

std::string GetLine(std::string& in, size_t& index)
{
	std::string out;
	while (index < in.size())
	{
		char c = in.c_str()[index];
		out += c;
		if (c == '0' || c == '\n')
		{
			if (c == '\n') out.pop_back();
			break;
		}
		index++;
	}
	return out;
}

void LowRenderer::GameUI::UIText::RenderGameUI(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& v, const Core::Maths::Vec2 ScreenRes, const Core::Maths::Vec2 MousePos, float ScrollValue, unsigned int MouseInputs)
{
	UIElement::RenderGameUI(VAOCurrent, shaderProgramCurrent, v, ScreenRes, MousePos, ScrollValue, MouseInputs);
	if (!Plane || !MFont) return;
	if (Label.c_str()[0] && lines.size() > 0)
	{
		TextMat = Core::Maths::Mat4::CreateTransformMatrix(ElementMat.GetPositionFromTranslation(), 0, 0.01f * TextSize);
		Core::Maths::Mat4 mv2 = v * TextMat;
		glUniform1i((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::Texture), MFont->BindForRender());
		glUniformMatrix4fv((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MVP), 1, GL_FALSE, mv2.content);
		glUniform3f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAmbient), TextColor.x, TextColor.y, TextColor.z);
		glUniform1f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAlpha), -1);
		glDisable(GL_BLEND);
		Core::Maths::Vec2 pos;
		for (size_t n = 0; n < lines.size(); n++)
		{
			if (!lines[n].line.c_str()[0]) continue;
			pos = Core::Maths::Vec2(-(((1.0f - 1.0f / lines[n].line.size()) * lines[n].TotalLength - 1 / 100.0f) * 0.5f * TextSize * ScreenRes.y / ScreenRes.x), (lines.size()-1) / 200.0f * TextSize * 2.2f - (n / 100.0f * TextSize * 2.2f));
			for (size_t i = 0; i < lines[n].line.size(); i++)
			{
				unsigned char c = lines[n].line.data()[i];
				if (c == '\n')
				{
					break;
				}
				glUniform2f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaPos), pos.x, pos.y);
				glUniform2f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaUV), (c % 16) / 16.0f, (c / 16) / 16.0f);
				Plane->Draw(VAOCurrent);
				pos.x += ((1 / 100.0f * 2.0f * MFont->GetSpacing(c)) + 1 / 400.0f) * TextSize * ScreenRes.y / ScreenRes.x;
			}
		}
		glEnable(GL_BLEND);
	}
	
}

void LowRenderer::GameUI::UIText::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float DeltaTime)
{
	if (!Plane) Plane = resources->Get<Resources::Mesh>("DebugPlane");
	if (!MFont)
	{
		MFont = resources->Get<Resources::Font>("DefaultResources/Font/default_font.png");
		UpdateText();
	}
}

void LowRenderer::GameUI::UIText::OnClick()
{
}

void LowRenderer::GameUI::UIText::UpdateText()
{
	lines.clear();
	if (!MFont) return;
	size_t index = 0;
	while (index < Label.size())
	{
		LineData data;
		data.line = GetLine(Label, index);
		data.TotalLength = 0.0f;
		for (size_t i = 0; i < data.line.size(); i++) data.TotalLength += ((1 / 100.0f * 2.0f * MFont->GetSpacing(data.line.c_str()[i])) + 1 / 400.0f);
		index++;
		lines.push_back(data);
	}
}

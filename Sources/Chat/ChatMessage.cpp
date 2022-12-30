#include "Chat/ChatMessage.hpp"

#include <glad/glad.h>

#include "Resources/Font.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/ShaderProgram.hpp"

namespace Chat
{
	void ChatMessage::Render(ChatRenderData& data)
	{
		float posX = data.cursor.x;
		data.cursor.y += 1 / 100.0f * data.textSize * 2.2f * (mLineCount-1);
		float currentWidth = 0.0f;
		for (size_t n = 0; n < mData.size(); n++)
		{
			ChatTextElement& elem = mData[n];
			glUniform3f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAmbient), elem.mColor.x, elem.mColor.y, elem.mColor.z);
			//glUniform1f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAlpha), color.w);
			if (!mData[n].mText.c_str()[0]) continue;
			for (size_t i = 0; i < elem.mText.size(); i++)
			{
				unsigned char c = elem.mText.data()[i];
				if (c == '\n')
				{
					break;
				}
				glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaPos), data.cursor.x, data.cursor.y);
				glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaUV), (c % 16) / 16.0f, (c / 16) / 16.0f);
				data.mesh->Draw(data.VAOCurrent);
				float width = ((2 / 100.0f * data.font->GetSpacing(c)) + 1 / 400.0f);
				currentWidth += width;
				if (currentWidth <= data.lineWidth)
				{
					data.cursor.x += width * data.textSize * data.aspectRatio;
				}
				else
				{
					currentWidth = 0.0f;
					data.cursor.x = posX;
					data.cursor.y -= 1 / 100.0f * data.textSize * 2.2f;
					data.lines++;
				}
			}
		}
	}

	void ChatMessage::RefreshLines(Resources::Font* font, f32 maxWidth)
	{
		f32 lineLength = 0.0f;
		mLineCount = 1;
		auto ResetLine = [&]()
		{
			mLineCount++;
			lineLength = 0.0f;
		};
		for (u64 i = 0; i < mData.size(); i++)
		{
			std::string& elem = mData[i].mText;
			for (u64 t = 0; t < elem.size(); t++)
			{
				const char& c = elem.at(t);
				if (c == '\n')
				{
					ResetLine();
				}
				float width = ((2 / 100.0f * font->GetSpacing(c)) + 1 / 400.0f);
				lineLength += width;
				if (lineLength > maxWidth)
				{
					ResetLine();
					lineLength = width;
				}
			}
		}
	}

	u32 ChatMessage::GetLineCount()
	{
		return mLineCount;
	}
}
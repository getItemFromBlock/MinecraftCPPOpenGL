#include "Chat/ChatHandler.hpp"

#include <glad/glad.h>
#include <ImGUI/imgui.h>

#include "Resources/Font.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Core/App/App.hpp"

namespace Chat
{
	Resources::Mesh* ChatHandler::mMesh = nullptr;
	Resources::Font* ChatHandler::mFont = nullptr;

	ChatHandler::ChatHandler()
	{
		mBindings = Core::App::App::GetInputBindings();
		mInputs = Core::App::App::GetInputs();
	}

	void ChatHandler::Render(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent)
	{
		//glDisable(GL_BLEND);
		mUICam.fov = 1;
		mUICam.Update(mInputs->ScreenSize, Core::Maths::Vec3(), Core::Maths::Vec3(0, 0, 1), Core::Maths::Vec3(0, 1, 0));
		Mat4 TextMat = Core::Maths::Mat4::CreateScaleMatrix(0.01f * mTextWidth);
		Core::Maths::Mat4 mv2 = mUICam.GetOrthoMatrix() * TextMat;
		glUniform1i((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::Texture), mFont->GetTextureID());
		glUniformMatrix4fv((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MVP), 1, GL_FALSE, mv2.content);
		glUniform1f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAlpha), 1);
		ChatMessage::ChatRenderData data =
		{
			VAOCurrent,
			shaderProgramCurrent,
			mMesh,
			mFont,
			Core::Maths::Vec2(-0.95f, mMessages.size() == 0 ? 0.5f : -0.87f + mMessages.back().GetLineCount() * 0.05f),
			mInputs->ScreenSize.y * 1.0f / mInputs->ScreenSize.x,
			mTextWidth,
			mLineWidth,
			0,
		};
		if (mChatMode)
		{
			data.cursor.y -= mScrollValue;
			for (s64 i = mMessages.size() - 1; i >= 0; i--)
			{
				if (data.cursor.y > 1) break;
				mMessages[i].Render(data);
				data.cursor.x = -0.95f;
				data.cursor.y += 1 / 100.0f * data.textSize * 2.2f * mMessages[i].GetLineCount();
			}
			data.cursor = Vec2(-0.92f, -0.95f);
			glUniform3f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAmbient), 1.0f, 1.0f, 1.0f);
			for (size_t i = 0; i < mText.size(); i++)
			{
				unsigned char c = mText.data()[i];
				glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaPos), data.cursor.x, data.cursor.y);
				glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaUV), (c % 16) / 16.0f, (c / 16) / 16.0f);
				data.mesh->Draw(data.VAOCurrent);
				float width = ((1 / 100.0f * 2.0f * mFont->GetSpacing(c)) + 1 / 400.0f);
				if (i == mCursorPos)
				{
					c = '|';
					glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaPos), data.cursor.x-0.01f, data.cursor.y);
					glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaUV), (c % 16) / 16.0f, (c / 16) / 16.0f);
					data.mesh->Draw(data.VAOCurrent);
				}
				data.cursor.x += (width * data.textSize * data.aspectRatio);
			}
			if (mCursorPos == mText.length())
			{
				unsigned char c = '_';
				glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaPos), data.cursor.x, data.cursor.y);
				glUniform2f((*data.shaderProgramCurrent)->GetLocation(Resources::ShaderData::DeltaUV), (c % 16) / 16.0f, (c / 16) / 16.0f);
				data.mesh->Draw(data.VAOCurrent);
			}
		}
		else
		{
			
			for (s64 i = mMessages.size() - 1; i >= 0; i--)
			{
				if (mMessages[i].mLifetime <= 0.0f || data.lines > 10)
				{
					break;
				}
				glUniform1f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAlpha), Core::Maths::Util::minF(mMessages[i].mLifetime, 1.0f));
				mMessages[i].Render(data);
				data.cursor.x = -0.95f;
				data.cursor.y += 1 / 100.0f * data.textSize * 2.2f * mMessages[i].GetLineCount();
			}
		}
		glUniform1f((*shaderProgramCurrent)->GetLocation(Resources::ShaderData::MatAlpha), 1);
		//glEnable(GL_BLEND);
	}

	void ChatHandler::Update(float deltaTime)
	{
		for (s64 i = mMessages.size() - 1; i >= 0; i--)
		{
			mMessages[i].mLifetime -= deltaTime;
			if (mMessages[i].mLifetime <= 0.0f)
			{
				break;
			}
		}
		if (!mChatMode)
		{
			if (!mInputs->mouseCaptured) return;
			bool chat = ImGui::IsKeyPressed(mBindings[static_cast<unsigned int>(Core::App::InputType::Chat)]);
			bool com = ImGui::IsKeyPressed(mBindings[static_cast<unsigned int>(Core::App::InputType::Command)]);
			if (chat || com)
			{
				mChatMode = true;
				if (com)
				{
					mText = "/";
					mCursorPos = 1;
				}
				else
				{
					mText.clear();
					mCursorPos = 0;
				}
				mHistoryPos = 0;
				mInputs->ClearLastKeys();
				mInputs->SetTypingEnabled(true);
			}
		}
		else
		{
			mScrollValue += mInputs->scroll * 0.1f;
			if (mScrollValue < 0.0f) mScrollValue = 0.0f;
			std::string typedChars = mInputs->GetLastKeys();
			mCursorPos = Core::Maths::Util::minI(mCursorPos, (s32)mText.length());
			if (typedChars.length() != 0)
			{
				mText.insert(mCursorPos, typedChars);
				mCursorPos += (u32)typedChars.length();
			}
			mInputs->ClearLastKeys();
			if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && mCursorPos != 0)
			{
				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
				{
					while (mCursorPos != 0 && mText[mCursorPos] == ' ') mCursorPos--;
					while (mCursorPos != 0 && mText[mCursorPos] != ' ') mCursorPos--;
				}
				else
				{
					mCursorPos--;
				}
			}
			if (ImGui::IsKeyPressed(ImGuiKey_RightArrow) && mText.length() != 0 && mCursorPos < mText.length())
			{
				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
				{
					while (mCursorPos < mText.length() && mText[mCursorPos] == ' ') mCursorPos++;
					while (mCursorPos < mText.length() && mText[mCursorPos] != ' ') mCursorPos++;
				}
				else
				{
					mCursorPos++;
				}
			}
			if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
			{
				readFromHistory(true);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
			{
				readFromHistory(false);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Backspace) && mText.size() != 0 && mCursorPos != 0)
			{
				auto eraseChar = [&]()
				{
					mText.erase(mCursorPos - 1, 1);
					mCursorPos--;
				};
				if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))
				{
					while (mCursorPos != 0 && mText[mCursorPos - 1] == ' ') eraseChar();
					while (mCursorPos != 0 && mText[mCursorPos - 1] != ' ') eraseChar();
					if (mCursorPos != 0) eraseChar();
				}
				else
				{
					eraseChar();
				}
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Delete) && mText.size() != 0 && mCursorPos != mText.length())
			{
				mText.erase(mCursorPos, 1);
			}
			bool enter = ImGui::IsKeyPressed(ImGuiKey_Enter);
			bool esc = ImGui::IsKeyPressed(ImGuiKey_Escape);
			if (enter || esc)
			{
				if (enter)
				{
					writeToHistory();
					// TODO
					ChatMessage newMes;
					newMes.mData.push_back(ChatTextElement(mText.c_str(), Vec3(1)));
					newMes.RefreshLines(mFont, mLineWidth);
					mMessages.push_back(std::move(newMes));
				}
				mChatMode = false;
				mInputs->SetTypingEnabled(false);
			}
		}
	}

	bool ChatHandler::IsFocused()
	{
		return mChatMode;
	}

	void ChatHandler::SetData(Resources::Mesh* mesh, Resources::Font* font)
	{
		mMesh = mesh;
		mFont = font;
	}

	void ChatHandler::writeToHistory()
	{
		mHistory[mHistoryCursor] = mText;
		mHistoryCursor++;
		mHistorySize++;
		if (mHistoryCursor >= mHistoryMaxSize) mHistoryCursor -= mHistoryMaxSize;
	}

	void ChatHandler::readFromHistory(bool old)
	{
		if (mHistorySize == 0) return;
		if (mHistoryPos == 0)
		{
			mTextTemp = mText;
		}
		if (old)
		{
			if (mHistoryPos < mHistorySize) mHistoryPos++;
		}
		else
		{
			if (mHistoryPos > 0) mHistoryPos--;
		}
		s32 total = (s32)mHistoryCursor - mHistoryPos;
		if (total < 0) total += mHistoryMaxSize;
		if (mHistoryPos == 0) mText = mTextTemp;
		else mText = mHistory[total];
		mCursorPos = (u32)mText.length();
	}
}


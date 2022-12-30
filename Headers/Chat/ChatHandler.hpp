#pragma once

#include <array>

#include "Chat/ChatMessage.hpp"
#include "LowRenderer/Rendering/Camera.hpp"

namespace Core
{
	namespace App
	{
		class Inputs;
	}
}

namespace Chat
{
	class ChatHandler
	{
	public:
		ChatHandler();
		~ChatHandler() = default;

		void Render(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent);
		void Update(float deltaTime);
		bool IsFocused();

		static void SetData(Resources::Mesh* mesh, Resources::Font* font);
	private:
		static Resources::Mesh* mMesh;
		static Resources::Font* mFont;

		LowRenderer::Rendering::Camera mUICam;

		bool mChatMode = false;
		float mScrollValue = 0.0f;
		static constexpr f32 mTextWidth = 3.0f;
		static constexpr f32 mLineWidth = 0.3f;
		std::string mText = "";
		std::string mTextTemp = "";
		u32 mCursorPos = 0;
		static constexpr u8 mHistoryMaxSize = 32;
		std::array<std::string, mHistoryMaxSize> mHistory;
		u8 mHistoryCursor = 0;
		u8 mHistorySize = 0;
		u8 mHistoryPos = 0;
		std::vector<ChatMessage> mMessages;
		Core::App::Inputs* mInputs = nullptr;
		int* mBindings = nullptr;

		void writeToHistory();
		void readFromHistory(bool old);
	};
}
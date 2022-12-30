#pragma once

#include <string>
#include <vector>

#include "Core/Util/Types.hpp"
#include "Chat/ChatTextElement.hpp"

namespace Resources
{
	class Mesh;
	class Font;
	class ShaderProgram;
}

namespace Chat
{
	class ChatMessage
	{
	public:
		struct ChatRenderData
		{
			unsigned int& VAOCurrent;
			Resources::ShaderProgram** shaderProgramCurrent;
			Resources::Mesh* mesh;
			Resources::Font* font;
			Core::Maths::Vec2 cursor;
			const f32 aspectRatio;
			const f32 textSize;
			const f32 lineWidth;
			int lines;
		};
		ChatMessage() = default;
		~ChatMessage() = default;

		void Render(ChatRenderData& data);

		u32 GetLineCount();
		void RefreshLines(Resources::Font* font, f32 maxWidth);

		f32 mLifetime = 10.0f;
		std::vector<ChatTextElement> mData;
	private:
		u32 mLineCount = 0;
	};

}

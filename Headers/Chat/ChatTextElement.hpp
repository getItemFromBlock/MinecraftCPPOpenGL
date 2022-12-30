#pragma once

#include <string>

#include "Core/Maths/Maths.hpp"
#include "Core/Util/Types.hpp"

namespace Chat
{
	class ChatTextElement
	{
	public:
		ChatTextElement() = default;
		ChatTextElement(const char* text, Core::Maths::Vec3 color) : mText(text), mColor(color) {}
		~ChatTextElement() = default;
		
		std::string mText;
		Core::Maths::Vec3 mColor = Core::Maths::Vec3(1.0f);
	};
}
#pragma once

#include <thread>
#include <string>

#include "Resources/IResource.hpp"

namespace Core::Util
{
	class ThreadHandler
	{
	public:
		ThreadHandler();
		~ThreadHandler();

		Signal locked;
		Signal shouldExit;
		void SetResource(Resources::IResource* resIn, const char* pathIn);
		Resources::IResource* res;
	private:
		std::string path;
		std::thread td;

		void ThreadFunc();
	};
}

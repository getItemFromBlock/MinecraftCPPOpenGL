#include "Core/Util/ThreadHandler.hpp"

#include "Networking/Network.hpp"

Core::Util::ThreadHandler::ThreadHandler()
{
	res = nullptr;
	locked.Store(true);
	shouldExit.Store(false);
	td = std::thread(&Core::Util::ThreadHandler::ThreadFunc, this);
}

Core::Util::ThreadHandler::~ThreadHandler()
{
	if (td.joinable())
	{
		shouldExit.Store(true);
		td.join();
	}
}

void Core::Util::ThreadHandler::SetResource(Resources::IResource* resIn, const char* pathIn)
{
	res = resIn;
	path = pathIn;
}

void Core::Util::ThreadHandler::ThreadFunc()
{
	while (!shouldExit.Load())
	{
		while (locked.Load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		if (res && !res->IsLoaded() && path.c_str()[0]) res->Load(path.c_str());
		locked.Store(true);
	}
}

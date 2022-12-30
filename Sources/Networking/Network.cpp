#include "Networking/Network.hpp"

#include "Networking/Sockets.hpp"
#include "Networking/Errors.hpp"
#include "Core/Debug/Log.hpp"

namespace Networking
{
	Network::Network()
	{
		if (!Networking::Start())
		{
			LOG("Network Error : %d", Networking::Sockets::GetError());
		}
		else
		{
			isValid = true;
		}
	}

	Network::~Network()
	{
		if (isValid)
		{
			Networking::Release();
		}
	}
}
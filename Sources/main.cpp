#include "Core/App/App.hpp"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "Core/Debug/Log.hpp"
#include "Core/Debug/Assert.hpp"
#include "Core/Maths/Maths.hpp"

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

// TODO remove
#include "Networking/Sockets.hpp"
#include "Networking/Errors.hpp"
#include "Networking/Network.hpp"
#include "Networking/Address.hpp"
#include "Networking/UDP/Client.hpp"
#include "Networking/UDP/Protocols/ReliableOrdered.hpp"
#include "Networking/Messages.hpp"
#include "Core/Util/Types.hpp"
#include <iostream>

int main(int argc, char** argv)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(856);
#endif
	{
		Core::App::AppInit data = { 1200, 900, 4, 5, "Minecraft C++", Core::Debug::Log::glDebugOutput };
		Core::App::App app = Core::App::App();
		Core::Debug::Log::OpenFile("Logs/output");

		int out = app.InitApp(data);
		if (out) return out;

		Core::Debug::Log::Print("OPENGL Version: %s\n", glGetString(GL_VERSION));

		app.Update();

		app.ClearApp();
		Core::Debug::Log::CloseFile();
	}
	/*
	Networking::Network network;
	if (!network.isValid) return -1;

	const Networking::Address client1 = Networking::Address::Loopback(Networking::Address::Type::IPv4, 8888);
	const Networking::Address client2 = Networking::Address::Loopback(Networking::Address::Type::IPv4, 9999);

	const std::vector<std::string> messagesToSend = { "my", "first", "udp", "transfert" };
	std::mutex coutMutex;
	// Create a thread per client
	std::thread t1([&]()
		{
			Networking::UDP::Client client;
			client.registerChannel<Networking::UDP::Protocols::ReliableOrdered>();
			client.simulator().enable();
			if (!client.init(client1.port()))
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 1 initialisation error : " << Networking::Sockets::GetError();
				return;
			}
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 1 initialized on port " << client1.port() << std::endl;
			}
			// Connect client 1 to client 2
			client.connect(client2);
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 1 connecting to " << client2.toString() << "..." << std::endl;
			}
			std::vector<std::string> receivedMessages;
			for (bool exit = false; !exit;)
			{
				client.receive();
				auto messages = client.poll();
				for (auto&& message : messages)
				{
					if (message->is<Networking::Messages::Connection>())
					{
						if (message->emitter() != client2)
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Unexpected connection from " << message->emitter().toString() << " (should be from " << client2.toString() << ")" << std::endl;
							continue;
						}
						else
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Client 2 [" << client2.toString() << "] connected to client 1" << std::endl;
						}
					}
					else if (message->is<Networking::Messages::UserData>())
					{
						const Networking::Messages::UserData* userdata = message->as<Networking::Messages::UserData>();
						std::string msg;
						msg.resize(userdata->data.size());
						std::copy(userdata->data.data(), userdata->data.data() + userdata->data.size(), msg.data());
						receivedMessages.push_back(msg);
						if (receivedMessages == messagesToSend)
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Every messages received in order !" << std::endl;
							std::cout << "Disconnecting client 2..." << std::endl;
							client.disconnect(client2);
						}
					}
					else if (message->is<Networking::Messages::Disconnection>())
					{
						assert(message->emitter() == client2);
						std::cout << "Shutting down client 1..." << std::endl;
						exit = true;
					}
				}
				client.processSend();
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
			client.release();
		});
	std::thread t2([&]()
		{
			Networking::UDP::Client client;
			client.registerChannel<Networking::UDP::Protocols::ReliableOrdered>();
			client.simulator().enable();
			if (!client.init(client2.port()))
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 2 initialisation error : " << Networking::Sockets::GetError();
				return;
			}
			{
				std::scoped_lock lock(coutMutex);
				std::cout << "Client 2 initialized on port " << client2.port() << std::endl;
			}
			for (bool connected = false, exit = false; !exit;)
			{
				client.receive();
				auto messages = client.poll();
				for (auto&& message : messages)
				{
					if (message->is<Networking::Messages::IncomingConnection>())
					{
						if (message->emitter() != client1)
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Unexpected connection received from " << message->emitter().toString() << " (should be from " << client1.toString() << ")" << std::endl;
							client.disconnect(message->emitter());
							continue;
						}
						else
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Client 2 receiving incoming connection from [" << message->emitter().toString() << "] (client 1)... and accepting it" << std::endl;
						}
						client.connect(message->emitter());
					}
					else if (message->is<Networking::Messages::Connection>())
					{
						if (message->emitter() != client1)
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Unexpected connection from " << message->emitter().toString() << " (should be from " << client1.toString() << ")" << std::endl;
							continue;
						}
						else
						{
							std::scoped_lock lock(coutMutex);
							std::cout << "Client 1 [" << client1.toString() << "] connected to client 2" << std::endl;
						}
						// Send messages to client 1, 1 message per packet
						for (const auto& msg : messagesToSend)
						{
							std::vector<u8> buffer = std::vector<u8>(msg.size(), 0);
							std::copy(msg.data(), msg.data() + msg.size(), buffer.data());
							client.sendTo(client1, std::move(buffer), 0);
						}
						connected = true;
					}
					else if (connected)
					{
						// Wait for client 1 to disconnect
						if (message->is<Networking::Messages::Disconnection>())
						{
							std::scoped_lock lock(coutMutex);
							assert(message->emitter() == client1);
							const auto t = message->as<Networking::Messages::Disconnection>();
							if (t->reason == Networking::Messages::Disconnection::Reason::Disconnected)
								std::cout << "Disconnection from client 1... [Disconnected]" << std::endl;
							else
								std::cout << "Disconnection from client 1... [Lost]" << std::endl;
							exit = true;
						}
					}
				}
				client.processSend();
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
			std::cout << "Normal termination." << std::endl;
			client.release();
		});

	t1.join();
	t2.join();

	system("pause");
	*/
	return 0;
}
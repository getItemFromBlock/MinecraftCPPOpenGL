#pragma once

#include <inttypes.h>
#include <vector>
#include <functional>
#include <unordered_set>

#include "Networking/Sockets.hpp"
#include "Networking/Address.hpp"
#include "Networking/NetworkSettings.hpp"
#include "Networking/UDP/Simulator.hpp"
#include "Core/Debug/Assert.hpp"
#include "DistantClient.hpp"

namespace Networking
{
	namespace Messages {
		class Base;
	}

	namespace UDP
	{
		class DistantClient;

		class Client
		{
			friend class DistantClient;

		public:
			Client();
			Client(const Client&) = delete;
			Client(Client&&) = delete;
			Client& operator=(const Client&) = delete;
			Client& operator=(Client&&) = delete;
			~Client();

			template<class T>
			void registerChannel(u8 channelId = 0);

#if NETWORK_SIMULATOR
			Simulator& simulator() { return mSimulator; }
			const Simulator& simulator() const { return mSimulator; }
#endif

			// Initialise socket to send and receive data on the given port
			bool init(u16 port);
			void release();

			static void SetTimeout(std::chrono::milliseconds timeout);
			static std::chrono::milliseconds GetTimeout();

			// Can be called anytime from any thread
			void connect(const Address& addr);
			// Can be called anytime from any thread
			void disconnect(const Address& addr);
			// Can be called anytime from any thread
			void sendTo(const Address& target, std::vector<u8>&& data, u32 channelIndex);
			void sendTo(const Address& target, const u8* data, size_t dataSize, u32 channelIndex) { sendTo(target, std::vector<u8>(data, data + dataSize), channelIndex); }

			// This performs operations on existing clients. Must not be called while calling receive
			void processSend();
			// This performs operations on existing clients. Must not be called while calling processSend
			void receive();
			// Extract ready messages. Can be called anytime from any thread but each message is unique and polled only once
			std::vector<std::unique_ptr<Messages::Base>> poll();

#if NETWORK_INTERRUPTION
			inline void enableNetworkInterruption() { setNetworkInterruptionEnabled(true); }
			inline void disableNetworkInterruption() { setNetworkInterruptionEnabled(false); }
			inline void setNetworkInterruptionEnabled(bool enabled) { mNetworkInterruptionAllowed = enabled; }
			inline bool isNetworkInterruptionAllowed() const { return mNetworkInterruptionAllowed; }
			inline bool isNetworkInterrupted() const { return !mInterruptedClients.empty(); }
#endif

		private:
			DistantClient* getClient(const Address& clientAddr, bool create = false);
			void setupChannels(DistantClient& client);

		private:
			void onMessageReady(std::unique_ptr<Messages::Base>&& msg);

		private:
			SOCKET mSocket = INVALID_SOCKET;
			std::vector<std::unique_ptr<DistantClient>> mClients;
			u64 mClientIdsGenerator{ 0 };
			std::mutex mMessagesLock;
			using MessagesLock = std::lock_guard<decltype(mMessagesLock)>;
			std::vector<std::unique_ptr<Messages::Base>> mMessages;

			struct ChannelRegistration {
				std::function<void(DistantClient&)> creator;
				u8 channelId;
			};
			std::vector<ChannelRegistration> mRegisteredChannels;

			struct Operation {
			public:
				enum class Type {
					Connect,
					SendTo,
					Disconnect,
				};
			public:
				static Operation Connect(const Address& target) { return Operation(Type::Connect, target); }
				static Operation SendTo(const Address& target, std::vector<u8>&& data, u32 channel) { return Operation(Type::SendTo, target, std::move(data), channel); }
				static Operation Disconnect(const Address& target) { return Operation(Type::Disconnect, target); }

				Operation(Type type, const Address& target)
					: mType(type)
					, mTarget(target)
				{}
				Operation(Type type, const Address& target, std::vector<u8>&& data, u32 channel)
					: mType(type)
					, mTarget(target)
					, mData(std::move(data))
					, mChannel(channel)
				{}

				Type mType;
				Address mTarget;
				std::vector<u8> mData;
				u32 mChannel = 0;
			};
			std::mutex mOperationsLock;
			using OperationsLock = std::lock_guard<decltype(mOperationsLock)>;
			std::vector<Operation> mPendingOperations;

#if NETWORK_SIMULATOR
			Simulator mSimulator;
#endif
#if NETWORK_INTERRUPTION
			bool mNetworkInterruptionAllowed = true;
			void onClientInterrupted(const DistantClient* client);
			void onClientResumed(const DistantClient* client);
			// Returns whether this client is the sole responsible for the network interruption
			bool isInterruptionCulprit(const DistantClient* client) const;
			std::unordered_set<const DistantClient*> mInterruptedClients;
#endif
		};

		template<class T>
		void Client::registerChannel(u8 channelId)
		{
			Assert(mSocket == INVALID_SOCKET); // Don't add channels after being initialized !!!
			Assert(std::find_if(mRegisteredChannels.begin(), mRegisteredChannels.end(), [&](const ChannelRegistration& registration) { return registration.channelId == channelId; }) == mRegisteredChannels.end());
			mRegisteredChannels.push_back({ [channelId](DistantClient& distantClient) { distantClient.registerChannel<T>(channelId); }, channelId });
		}
	}
}
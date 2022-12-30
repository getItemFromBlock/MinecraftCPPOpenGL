#pragma once

#include <string>

#include "Core/Util/Types.hpp"
#include "Sockets.hpp"

namespace Networking
{
	class Address
	{
	public:
		enum class Type : u8 {
			None,
			IPv4,
			IPv6,
		};
	public:
		Address() = default;
		Address(const Address&);
		Address& operator=(const Address&);
		~Address() = default;

		Address(const std::string& ip, uint16_t port);
		Address(const sockaddr_storage& addr);

		static Address Any(Type type, u16 port);
		static Address Loopback(Type type, u16 port);

		bool operator==(const Address& other) const;
		bool operator!=(const Address& other) const { return !(*this == other); }

		Type type() const { return mType; }
		bool isValid() const { return mType != Type::None; }
		std::string address() const;
		uint16_t port() const { return mPort; }
		std::string toString() const;

		// Connecte le socket en param�tre � l�adresse interne
			// Retourne true si la connexion r�ussit ou d�bute (socket non bloquant), false sinon
		bool connect(SOCKET sckt) const;
		// Accepte une connexion entrante sur le socket en param�tre, puis met � jour l�adresse interne avec celle de l��metteur
		// Retourne true si un nouveau socket a �t� accept� et met � jour newClient avec le nouveau socket, false sinon
		bool accept(SOCKET sckt, SOCKET& newClient);
		// Assigne l�adresse interne au socket en param�tre
		bool bind(SOCKET sckt) const;
		// Envoie des donn�es depuis le socket en param�tre vers l�adresse interne
		int sendTo(SOCKET sckt, const char* data, size_t datalen) const;
		// Re�oit des donn�es depuis le socket en param�tre puis met � jour l�adresse interne avec celle de l��metteur
		int recvFrom(SOCKET sckt, u8* buffer, size_t bufferSize);

	private:
		void set(const sockaddr_storage& src);

		sockaddr_storage mStorage = { 0 };
		uint16_t mPort = 0;
		Type mType = Type::None;
	};
}
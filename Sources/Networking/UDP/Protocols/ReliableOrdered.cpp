#include "Networking/UDP/Protocols/ReliableOrdered.hpp"

#include "Core/Debug/Assert.hpp"

#include "Networking/Utils.hpp"

namespace Networking::UDP::Protocols
{
	void ReliableOrdered::RMultiplexer::queue(std::vector<u8>& msgData)
	{
		Assert(msgData.size() <= Packet::MaxMessageSize);
		if (msgData.size() > Packet::DataMaxSize)
		{
			u16 queuedSize = 0;
			while (queuedSize < msgData.size())
			{
				const auto fragmentSize = std::min(Packet::DataMaxSize, static_cast<uint16_t>(msgData.size() - queuedSize));
				mQueue.resize(mQueue.size() + 1);
				Packet& packet = mQueue.back().packet();
				packet.mHeader.id = mNextId++;
				packet.mHeader.type = ((queuedSize == 0) ? Packet::Type::FirstFragment : Packet::Type::Fragment);
				packet.mHeader.size = fragmentSize;
				memcpy(packet.data(), msgData.data() + queuedSize, fragmentSize);
				queuedSize += fragmentSize;
			}
			mQueue.back().packet().mHeader.type = Packet::Type::LastFragment;
			Assert(queuedSize == msgData.size());
		}
		else
		{
			mQueue.resize(mQueue.size() + 1);
			Packet& packet = mQueue.back().packet();
			packet.mHeader.id = mNextId++;
			packet.mHeader.type = Packet::Type::FullMessage;
			packet.mHeader.size = static_cast<uint16_t>(msgData.size());
			memcpy(packet.data(), msgData.data(), msgData.size());
		}
	}

#if NETWORK_INTERRUPTION
	u16 ReliableOrdered::RMultiplexer::serialize(u8* buffer, u16 buffersize, Datagram::ID datagramId, bool connectionInterrupted)
#else
	u16 ReliableOrdered::RMultiplexer::serialize(u8* buffer, u16 buffersize, Datagram::ID datagramId)
#endif
	{
#if NETWORK_INTERRUPTION
		// Don't send reliable data if the connection is interrupted
		if (connectionInterrupted)
			return 0;
#endif
		u16 serializedSize = 0;
		for (auto& packetHolder : mQueue)
		{
			//!< S�assurer avant tout que le paquet est dans les bornes d�envoi, sinon on peut arr�ter d�it�rer sur notre file
			if (!(Utils::SequenceDiff(packetHolder.packet().id(), mFirstAllowedPacket) < RDemultiplexer::QueueSize))
				break;
			if (!packetHolder.shouldSend())
				continue;
			const auto& packet = packetHolder.packet();
			if (serializedSize + packet.size() > buffersize)
				continue; //!< Si le paquet est trop gros, essayons d�inclure les suivants

			memcpy(buffer, packet.buffer(), packet.size());
			serializedSize += packet.size();
			buffer += packet.size();

			packetHolder.onSent(datagramId);
		}
		return serializedSize;
	}

	void ReliableOrdered::RMultiplexer::onDatagramAcked(Datagram::ID datagramId)
	{
		if (mQueue.empty())
			return;

		mQueue.erase(std::remove_if(mQueue.begin(), mQueue.end()
			, [&](const ReliablePacket& packetHolder) { return packetHolder.isIncludedIn(datagramId); })
			, mQueue.cend());
		if (mQueue.empty())
			mFirstAllowedPacket = mNextId; //!< Si la file est maintenant vide, la borne commence au prochain paquet mis en file
		else if (Utils::IsSequenceNewer(mQueue.front().packet().id(), mFirstAllowedPacket))
			mFirstAllowedPacket = mQueue.front().packet().id(); // Sinon, on d�place les bornes d�envoi au plus ancien paquet en file
	}


	void ReliableOrdered::RMultiplexer::onDatagramLost(Datagram::ID datagramId)
	{
		for (auto& packetHolder : mQueue)
		{
			if (packetHolder.isIncludedIn(datagramId))
				packetHolder.resend();
		}
	}

	void ReliableOrdered::RDemultiplexer::onDataReceived(const u8* data, u16 datasize)
	{
		//<! Extraire les paquets du tampon
		u16 processedDataSize = 0;
		while (processedDataSize < datasize)
		{
			const Packet* pckt = reinterpret_cast<const Packet*>(data);
			if (processedDataSize + pckt->size() > datasize || pckt->datasize() > Packet::DataMaxSize)
			{
				//<! Paquet ou tampon mal form�
				return;
			}
			onPacketReceived(pckt);
			processedDataSize += pckt->size();
			data += pckt->size();
		}
	}

	void ReliableOrdered::RDemultiplexer::onPacketReceived(const Packet* pckt)
	{
		if (!Utils::IsSequenceNewer(pckt->id(), mLastProcessed))
			return; //!< Paquet obsol�te

		//!< Calcul de l�index dans le tableau
		const size_t index = pckt->id() % mPendingQueue.size();
		Packet& pendingPacket = mPendingQueue[index];
		if (pendingPacket.datasize() == 0)
		{
			// Emplacement disponible, copier simplement les donn�es du r�seau dans notre tableau
			pendingPacket = *pckt;
		}
		else
		{
			// Emplacement NON disponible, s�assurer qu�il contient d�j� notre paquet, sinon il y a un probl�me
			Assert(pendingPacket.id() == pckt->id() && pendingPacket.datasize() == pckt->datasize());
		}
	}

	std::vector<std::vector<u8>> ReliableOrdered::RDemultiplexer::process()
	{
		//!< Fonction de r�initialisation d�un paquet
		auto ResetPacket = [](Packet& pckt) { pckt.mHeader.size = 0; };
		auto IsPacketValid = [](const Packet& pckt) { return pckt.mHeader.size != 0; };
		std::vector<std::vector<u8>> messagesReady;

		Packet::ID expectedPacketId = mLastProcessed + 1;
		//!< Il faut it�rer sur notre tableau en commen�ant par le paquet attendu, qui peut ne pas �tre en index 0
		const size_t startIndexOffset = expectedPacketId % mPendingQueue.size();
		for (size_t i = 0; i < mPendingQueue.size(); i++, expectedPacketId++)
		{
			//!< On calcule l�index dans notre tableau du prochain paquet � traiter
			const size_t packetIndex = (i + startIndexOffset) % mPendingQueue.size();
			Packet& packet = mPendingQueue[packetIndex];
			if (!IsPacketValid(packet))
				break;
			if (packet.type() == Packet::Type::FullMessage)
			{
				//!< Message complet
				std::vector<u8> msg(packet.data(), packet.data() + packet.datasize());
				mLastProcessed = packet.id();
				ResetPacket(packet);
				messagesReady.push_back(std::move(msg));
			}
			else if (packet.type() == Packet::Type::FirstFragment)
			{
				//!< V�rifier que le message est pr�t
				if (!isMessageFull(i, expectedPacketId, startIndexOffset))
					break; // Protocole ordonn� fiable : si le message suivant � extraire est incomplet, nous pouvons arr�ter le processus d�extraction

				// Nous avons un message fragment� complet, nous pouvons maintenant extraire les donn�es et r�initialiser chaque paquet utilis�
				std::vector<u8> msg(packet.data(), packet.data() + packet.datasize());
				i++;
				expectedPacketId++;
				// It�ration sur les paquets restants pour compl�ter le message
				for (size_t j = i; j < mPendingQueue.size(); i++, j++, expectedPacketId++)
				{
					const size_t idx = (j + startIndexOffset) % mPendingQueue.size();
					Packet& pckt = mPendingQueue[idx];

					if (pckt.type() == Packet::Type::LastFragment)
					{
						//!< Dernier fragment du message maintenant complet
						msg.insert(msg.cend(), pckt.data(), pckt.data() + pckt.datasize());
						mLastProcessed = pckt.id();
						ResetPacket(pckt);
						messagesReady.push_back(std::move(msg));
						break;
					}
					else if (pckt.type() != Packet::Type::Fragment)
					{
						//!< Paquet mal form� ou malicieux
						break;
					}

					msg.insert(msg.cend(), pckt.data(), pckt.data() + pckt.datasize());
					ResetPacket(pckt);
				}
			}
			else
			{
				// Protocole ordonn� fiable : si le message suivant � extraire est incomplet, nous pouvons arr�ter le processus d�extraction
				break;
			}
		}
		return messagesReady;
	}

	bool ReliableOrdered::RDemultiplexer::isMessageFull(size_t index, Networking::UDP::Datagram::ID packetID, const size_t& startIndexOffset) const
	{
		// On saute le premier fragment d�j� trait� par la boucle sur i
		++index;
		++packetID;
		// On it�re sur les paquets restants pour v�rifier que notre message soit complet
		for (size_t j = index; j < mPendingQueue.size(); ++j, ++packetID)
		{
			const size_t idx = (j + startIndexOffset) % mPendingQueue.size();
			const Packet& pckt = mPendingQueue[idx];
			if (pckt.id() != packetID || pckt.datasize() == 0)
				break; // Un paquet est manquant
			if (pckt.type() == Packet::Type::LastFragment)
			{
				//!< Nous avons atteint et re�u le dernier fragment, le message est complet
				return true;
			}
			else if (pckt.type() != Packet::Type::Fragment)
			{
				//!< Si nous arrivons ici nous avons probablement re�u un paquet mal form� ou malicieux
				break;
			}
		}
		return false;
	}

	void ReliableOrdered::queue(std::vector<u8>&& msgData)
	{
		multiplexer.queue(msgData);
	}

#if NETWORK_INTERRUPTION
	u16 ReliableOrdered::serialize(u8* buffer, u16 buffersize, Datagram::ID datagramId, bool connectionInterrupted)
	{
		return multiplexer.serialize(buffer, buffersize, datagramId, connectionInterrupted);
	}
#else
	u16 ReliableOrdered::serialize(u8* buffer, u16 buffersize, Datagram::ID datagramId)
	{
		return multiplexer.serialize(buffer, buffersize, datagramId);
	}
#endif

	void ReliableOrdered::onDatagramAcked(Datagram::ID datagramId)
	{
		multiplexer.onDatagramAcked(datagramId);
	}
	void ReliableOrdered::onDatagramLost(Datagram::ID datagramId)
	{
		multiplexer.onDatagramLost(datagramId);
	}

	void ReliableOrdered::onDataReceived(const u8* data, u16 datasize)
	{
		demultiplexer.onDataReceived(data, datasize);
	}

	std::vector<std::vector<u8>> ReliableOrdered::process()
	{
		return demultiplexer.process();
	}
}
#ifndef PACKET_HPP
#define PACKET_HPP

#include <list>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>

namespace ntwrk
{
    enum class packetType : sf::Uint8 { HI = 0, RECEIVE_CHECK_NEEDED = 1, CHECK_VALIDED = 2, LEVEL_CONTENT = 3 };

    struct packetInfo
    {
        sf::Packet dataPacket;
        bool checkReceiving = false;
        sf::Int16 idForCheck = -1;
    };

    struct clientInfo
    {
        sf::IpAddress ip;
        unsigned short portForSending = 0;
        unsigned short portForReceiving = 0;
        sf::Int16 lastIdUsedForPacketCheck = 0;
        std::list<packetInfo> listOfPacketsToSend;
    };

    void initSockets(sf::UdpSocket& senderSocket, sf::UdpSocket& receiverSocket, unsigned short portToListen);
    void addPacketToSend(sf::Packet& packetData, clientInfo& toThisClient, bool packetNeedReceiveCheck = false);
    void sendPackets(clientInfo& forThisClient, sf::UdpSocket& senderSocket);
    void removeCheckedSendedPacket(clientInfo& forThisClient, sf::Int16 idThatIsChecked);
    void processReceiveCheckNeededPacket(sf::Packet& thisPacket, clientInfo& forThisClient);
    void processCheckValidedPacket(sf::Packet& thisPacket, clientInfo& forThisClient);
}

#endif

#include <vector>
#include <SFML/Network/Packet.hpp>

#include "externalServer.hpp"

using packetType = ntwrk::packetType;

externalServerClass::externalServerClass()
{
    sf::Packet hiPacket;

    serverInfo.ip = "localhost";
    serverInfo.portForSending = 33888;

    ntwrk::initSockets(senderSocket, receiverSocket, sf::UdpSocket::AnyPort);

    hiPacket << static_cast<sf::Uint8>(packetType::HI) << sf::Uint16(receiverSocket.getLocalPort());

    ntwrk::addPacketToSend(hiPacket, serverInfo, true);
}

void externalServerClass::update()
{
    receivePackets();
    gamePlayed.update();
    ntwrk::sendPackets(serverInfo, senderSocket);
}

void externalServerClass::draw(sf::RenderWindow& window)
{
    gamePlayed.draw(window);
}

void externalServerClass::setInputKey(directionEnum direction)
{
    if(idOfPlayer != -1)
    {
        (void) direction;
        //sendpacketblabablabalbalb
    }
}

void externalServerClass::receivePackets()
{
    sf::Packet packetReceived;
    sf::IpAddress ipOfSender;
    unsigned short portOfSender;

    while(receiverSocket.receive(packetReceived, ipOfSender, portOfSender) == sf::UdpSocket::Done)
    {
        if(serverInfo.portForReceiving == 0)
        {
            serverInfo.portForReceiving = portOfSender;
        }

        if(ipOfSender == serverInfo.ip && portOfSender == serverInfo.portForReceiving)
        {
            processPacket(packetReceived);
        }
    }
}

void externalServerClass::processPacket(sf::Packet& thisPacket)
{
    packetType currentTypeOfPacket;
    sf::Uint8 currentTypeOfPacketInByte;

    thisPacket >> currentTypeOfPacketInByte;
    currentTypeOfPacket = static_cast<packetType>(currentTypeOfPacketInByte);

    switch(currentTypeOfPacket)
    {
        case packetType::RECEIVE_CHECK_NEEDED:
        {
            ntwrk::processReceiveCheckNeededPacket(thisPacket, serverInfo);
            processPacket(thisPacket);
            break;
        }
        case packetType::CHECK_VALIDED:
        {
            ntwrk::processCheckValidedPacket(thisPacket, serverInfo);
            break;
        }
        case packetType::LEVEL_CONTENT:
        {
            sf::Uint16 levelSize;
            std::vector<std::uint8_t> levelContent;

            thisPacket >> levelSize;

            for(int i = 0; i < levelSize; ++i)
            {
                sf::Uint8 chunkOfLevel;

                thisPacket >> chunkOfLevel;

                levelContent.push_back(chunkOfLevel);
            }

            gamePlayed.loadLevelFromCBOR(levelContent);
            break;
        }
        case packetType::HI:
        {
            break;
        }
    }
}

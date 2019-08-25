#include <SFML/Window/Event.hpp>

#include "serverState.hpp"
#include "entityBuilder.hpp"

using packetType = ntwrk::packetType;

serverStateClass::serverStateClass()
{
    ntwrk::initSockets(senderSocket, receiverSocket, 33888);
    levelContent = entityBuilderTool::getLevelContentInCBOR("level2");
    gamePlayed.loadLevelFromCBOR(levelContent);
}

void serverStateClass::update(sf::RenderWindow& window)
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
    }

    receivePackets();
    analyzePackets();
    gamePlayed.update();
    sendPackets();
}

void serverStateClass::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color(200, 200, 200));
    gamePlayed.draw(window);
}

void serverStateClass::sendPackets()
{
    for(playerInfo& thisPlayer : listOfPlayers)
    {
        ntwrk::sendPackets(thisPlayer.infoAsClient, senderSocket);
    }
}

void serverStateClass::receivePackets()
{
    sf::Packet packetReceived;
    sf::IpAddress ipOfSender;
    unsigned short portOfSender;

    while(receiverSocket.receive(packetReceived, ipOfSender, portOfSender) == sf::UdpSocket::Done)
    {
        bool packetWasProcessed = false;

        for(playerInfo& thisPlayer : listOfPlayers)
        {
            if(thisPlayer.infoAsClient.ip == ipOfSender && thisPlayer.infoAsClient.portForReceiving == portOfSender)
            {
                thisPlayer.listOfPacketsReceived.push_back(packetReceived);
                packetWasProcessed = true;
                break;
            }
        }

        if(packetWasProcessed == false)
        {
            listOfPlayers.push_back(playerInfo());
            listOfPlayers.back().infoAsClient.ip = ipOfSender;
            listOfPlayers.back().infoAsClient.portForReceiving = portOfSender;
            listOfPlayers.back().listOfPacketsReceived.push_back(packetReceived);
        }
    }
}

void serverStateClass::analyzePackets()
{
    for(playerInfo& thisPlayer : listOfPlayers)
    {
        while(thisPlayer.listOfPacketsReceived.empty() == false)
        {
            processPacket(thisPlayer.listOfPacketsReceived.front(), thisPlayer);
            thisPlayer.listOfPacketsReceived.pop_front();
        }
    }
}

void serverStateClass::processPacket(sf::Packet& thisPacket, playerInfo& forThisPlayer)
{
    packetType currentTypeOfPacket;
    sf::Uint8 currentTypeOfPacketInByte;

    thisPacket >> currentTypeOfPacketInByte;
    currentTypeOfPacket = static_cast<packetType>(currentTypeOfPacketInByte);

    switch(currentTypeOfPacket)
    {
        case packetType::RECEIVE_CHECK_NEEDED:
        {
            ntwrk::processReceiveCheckNeededPacket(thisPacket, forThisPlayer.infoAsClient);
            processPacket(thisPacket, forThisPlayer);
            break;
        }
        case packetType::HI:
        {
            sf::Packet newPacketToSend;
            sf::Uint16 newPortToUseForSending;

            thisPacket >> newPortToUseForSending;

            forThisPlayer.infoAsClient.portForSending = newPortToUseForSending;

            newPacketToSend << static_cast<sf::Uint8>(packetType::LEVEL_CONTENT) << static_cast<sf::Uint16>(levelContent.size());
            for(const std::uint8_t& levelChunk : levelContent)
            {
                newPacketToSend << levelChunk;
            }

            ntwrk::addPacketToSend(newPacketToSend, forThisPlayer.infoAsClient, true);
            break;
        }
        case packetType::CHECK_VALIDED:
        {
            ntwrk::processCheckValidedPacket(thisPacket, forThisPlayer.infoAsClient);
            break;
        }
        case packetType::LEVEL_CONTENT:
        {
            break;
        }
    }
}

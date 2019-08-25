#include "networkTool.hpp"

void ntwrk::initSockets(sf::UdpSocket& senderSocket, sf::UdpSocket& receiverSocket, unsigned short portToListen)
{
    if(receiverSocket.bind(portToListen) != sf::UdpSocket::Done)
    {
        //erreur
    }

    senderSocket.setBlocking(false);
    receiverSocket.setBlocking(false);
}

void ntwrk::addPacketToSend(sf::Packet& packetData, clientInfo& toThisClient, bool packetNeedReceiveCheck)
{
    packetInfo packetToSend;

    if(packetNeedReceiveCheck == true)
    {
        packetToSend.checkReceiving = true;
        packetToSend.idForCheck = toThisClient.lastIdUsedForPacketCheck++;
        packetToSend.dataPacket << static_cast<sf::Uint8>(packetType::RECEIVE_CHECK_NEEDED) << packetToSend.idForCheck;
    }

    if(packetData.getData() != nullptr)
    {
        packetToSend.dataPacket.append(packetData.getData(), packetData.getDataSize());
    }

    toThisClient.listOfPacketsToSend.push_back(packetToSend);
}

void ntwrk::sendPackets(clientInfo& forThisClient, sf::UdpSocket& senderSocket)
{
    std::list<packetInfo> listOfPacketsToAppendAtEnd;

    while(forThisClient.listOfPacketsToSend.empty() == false)
    {
        sf::UdpSocket::Status currentSendStatus = senderSocket.send(forThisClient.listOfPacketsToSend.front().dataPacket, forThisClient.ip, forThisClient.portForSending);

        if(forThisClient.listOfPacketsToSend.front().checkReceiving == true || currentSendStatus != sf::UdpSocket::Done)
        {
            listOfPacketsToAppendAtEnd.push_back(forThisClient.listOfPacketsToSend.front());
        }

        forThisClient.listOfPacketsToSend.pop_front();
    }

    for(packetInfo& thisPacketInfo : listOfPacketsToAppendAtEnd)
    {
        forThisClient.listOfPacketsToSend.push_back(thisPacketInfo);
    }
}

void ntwrk::removeCheckedSendedPacket(clientInfo& forThisClient, sf::Int16 idThatIsChecked)
{
    for(std::list<packetInfo>::iterator ite = forThisClient.listOfPacketsToSend.begin(); ite != forThisClient.listOfPacketsToSend.end(); ++ite)
    {
        if(ite->checkReceiving == true && ite->idForCheck == idThatIsChecked)
        {
            forThisClient.listOfPacketsToSend.erase(ite);
            break;
        }
    }
}

void ntwrk::processReceiveCheckNeededPacket(sf::Packet& thisPacket, clientInfo& forThisClient)
{
    sf::Int16 idOfCheck;
    sf::Packet newPacket;

    thisPacket >> idOfCheck;

    newPacket << static_cast<sf::Uint8>(packetType::CHECK_VALIDED) << idOfCheck;
    addPacketToSend(newPacket, forThisClient);
}

void ntwrk::processCheckValidedPacket(sf::Packet& thisPacket, clientInfo& forThisClient)
{
    sf::Int16 idOfCheck;

    thisPacket >> idOfCheck;

    removeCheckedSendedPacket(forThisClient, idOfCheck);
}

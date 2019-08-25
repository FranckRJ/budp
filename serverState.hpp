#ifndef SERVERSTATE_HPP
#define SERVERSTATE_HPP

#include <list>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "gameState.hpp"
#include "gamePlayed.hpp"
#include "networkTool.hpp"

struct playerInfo
{
    ntwrk::clientInfo infoAsClient;
    std::list<sf::Packet> listOfPacketsReceived;
};

class serverStateClass : public gameStateClass
{
public:
    serverStateClass();
    void update(sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
private:
    void sendPackets();
    void receivePackets();
    void analyzePackets();
    void processPacket(sf::Packet& thisPacket, playerInfo& forThisPlayer);
private:
    std::vector<std::uint8_t> levelContent;
    gamePlayedClass gamePlayed;
    sf::UdpSocket senderSocket;
    sf::UdpSocket receiverSocket;
    std::list<playerInfo> listOfPlayers;
};

#endif

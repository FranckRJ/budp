#ifndef EXTERNALSERVER_HPP
#define EXTERNALSERVER_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "server.hpp"
#include "gamePlayed.hpp"
#include "component.hpp"
#include "networkTool.hpp"

class externalServerClass : public serverClass
{
public:
    externalServerClass();
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void setInputKey(directionEnum direction) override;
private:
    void receivePackets();
    void processPacket(sf::Packet& thisPacket);
private:
    ntwrk::clientInfo serverInfo;
    sf::UdpSocket senderSocket;
    sf::UdpSocket receiverSocket;
    gamePlayedClass gamePlayed;
    int idOfPlayer = -1;
};

#endif

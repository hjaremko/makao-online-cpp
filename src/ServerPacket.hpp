#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include "Server.hpp"

namespace makao
{

    class ServerPacket : public sf::Packet
    {
        public:
            friend ServerPacket& operator<<( ServerPacket&, const Server& );

            ServerPacket();

            bool send( sf::UdpSocket*, sf::IpAddress, unsigned short );
    };
}

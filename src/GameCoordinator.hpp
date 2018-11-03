#pragma once

#include <vector>
#include <iostream>

#include <SFML/Network.hpp>

#include "Server.hpp"

namespace makao
{
    class GameCoordinator
    {
        public:
            GameCoordinator();
            ~GameCoordinator();

            void listen();
            int getActiveServers() const;
            bool bindSocket( const unsigned short );
            bool receive( Server& );
        
        private:
            sf::UdpSocket m_socket;
            sf::SocketSelector m_selector;
            std::vector<sf::UdpSocket*> m_sockets;
            std::vector<Server> m_activeServers;
    };
}

#pragma once

#include <vector>
#include <iostream>
#include <SFML/Network.hpp>
#include "ncwindows"
#include "Server.hpp"
#include "ServerPacket.hpp"

namespace makao
{
    class GameCoordinator
    {
        public:
            GameCoordinator( ncwindows::LogWindow* );
            ~GameCoordinator();

            bool bindSocket( const unsigned short );
            bool receive( auto& );
            void listen();
            int getActiveServers() const;
        
        private:
            sf::UdpSocket m_socket;
            std::vector<sf::UdpSocket*> m_sockets;
            std::vector<Server> m_activeServers;
            sf::SocketSelector m_selector;
            ncwindows::LogWindow* m_log;
    };
}
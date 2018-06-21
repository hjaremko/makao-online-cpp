#pragma once

#include <iostream>
#include <string>
#include <SFML/Network.hpp>

namespace makao
{
    class ServerPacket;
    
    class Server
    {
        public:
            std::string name;
            sf::IpAddress ip;
            unsigned short port;
            int freeSlots = 4;

            Server();
            Server( sf::IpAddress, unsigned short, std::string );
            Server( sf::IpAddress, unsigned short, std::string, int );

            bool operator==( const Server );
            std::string getInfo() const;
            float getElapsedTime() const;
            void setIp( std::string );
            void restartClock();
            void getFromPacket( ServerPacket& );

        private:
            sf::Clock m_timeout;
    };
}
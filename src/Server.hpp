#pragma once

#include <iostream>
#include <string>

#include <SFML/Network.hpp>

namespace makao
{
    class Server
    {
        public:
            Server();
            Server( sf::IpAddress, unsigned short, std::string, int = 4 );

            bool operator==( const Server& );
            std::string getInfo() const;
            float getElapsedTime() const;
            void setIp( std::string );
            void restartClock();

            sf::IpAddress ip;
            unsigned short port;
            std::string name;
            int maxSlots;
            int takenSlots;

        private:
            sf::Clock m_timeout;
    };
}

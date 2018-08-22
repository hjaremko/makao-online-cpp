#pragma once

#include <iostream>
#include <string>

#include <SFML/Network.hpp>

namespace makao
{
    class Server
    {
        public:
            std::string name;
            sf::IpAddress ip;
            unsigned short port;
            int takenSlots;
            int maxSlots;

            Server();
            Server( sf::IpAddress, unsigned short, std::string, int = 4 );

            bool operator==( const Server& );
            std::string getInfo() const;
            float getElapsedTime() const;
            void setIp( std::string );
            void restartClock();

        private:
            sf::Clock m_timeout;
    };
}

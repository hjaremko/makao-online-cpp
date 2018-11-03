#pragma once

#include <iostream>
#include <string>

#include <SFML/Network.hpp>

using ushort = unsigned short;

namespace makao
{
    class Server
    {
        public:
            Server();
            Server( sf::IpAddress, unsigned short, std::string, int = 4 );

            bool operator==( const Server& ) const;
            std::string getInfo() const;
            float getElapsedTime() const;
            void setIp( std::string );
            void restartClock();

            sf::IpAddress ip;
            ushort        port;
            std::string   name{ "blank" };
            int           maxSlots{ 0 };
            int           takenSlots{ 0 };

        private:
            sf::Clock m_timeout;
    };
}

sf::Packet& operator<<( sf::Packet&, const makao::Server& );
sf::Packet& operator>>( sf::Packet&, makao::Server& );

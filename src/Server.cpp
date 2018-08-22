#include "Server.hpp"
#include "ServerPacket.hpp"

namespace makao
{
    Server::Server()
    {

    }

    Server::Server( sf::IpAddress t_ip, unsigned short t_port, std::string t_name, int t_slots )
                  : ip( t_ip ), port( t_port ), name( t_name ), maxSlots( t_slots ), takenSlots( 0 )
    {

    }

    bool Server::operator==( const Server& t_right )
    {
        if ( this->name == t_right.name && this->ip == t_right.ip )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void Server::setIp( std::string t_ip )
    {
        ip = t_ip;
    }

    std::string Server::getInfo() const
    {
        return ( name + " " + std::to_string( takenSlots ) + "/" + std::to_string( maxSlots ) ); 
    }

    float Server::getElapsedTime() const
    {
        return m_timeout.getElapsedTime().asSeconds();
    }

    void Server::restartClock()
    {
        m_timeout.restart();
    }
}

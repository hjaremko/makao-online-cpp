#include "Server.hpp"

using ushort = unsigned short;

namespace makao
{
    Server::Server()
    {

    }

    Server::Server( sf::IpAddress t_ip, ushort t_port, std::string t_name, int t_slots )
                  : ip( t_ip ), port( t_port ), name( t_name ), maxSlots( t_slots ), takenSlots( 0 )
    {

    }

    bool Server::operator==( const Server& t_right ) const
    {
        return ( this->name == t_right.name && this->ip == t_right.ip );
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

sf::Packet& operator<<( sf::Packet& t_packet, const makao::Server& t_server )
{
    // std::cout << "Sending: " << t_server.getInfo() << std::endl;

    return t_packet << t_server.ip.toString() << t_server.port
                    << t_server.name << t_server.takenSlots << t_server.maxSlots;
}

sf::Packet& operator>>( sf::Packet& t_packet, makao::Server& t_server )
{

    std::string tmpIp;

    t_packet >> tmpIp >> t_server.port >> t_server.name
             >> t_server.takenSlots >> t_server.maxSlots;
    t_server.ip = tmpIp;

    // std::cout << "Receiving: " << t_server.getInfo() << std::endl;

    return t_packet;
}

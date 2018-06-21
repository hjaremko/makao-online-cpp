#include "ServerPacket.hpp"


namespace makao
{
    ServerPacket& operator<<( ServerPacket& t_packet, const Server& t_server )
    {
        t_packet << t_server.ip.toString() << t_server.port 
                 << t_server.name << t_server.freeSlots;
                 
        // std::cerr << t_server.name << std::endl;

        return t_packet;
    }

    ServerPacket::ServerPacket()
    {

    }

    bool ServerPacket::send( sf::UdpSocket* t_socket, sf::IpAddress t_receiver, unsigned short t_port )
    {
        if ( t_socket->send( *this, t_receiver, t_port ) != sf::Socket::Done )
        {
            std::cerr << "Error sending packet." << std::endl;
            return false;
        }
        else
        {
            // std::cerr << "Send!" << std::endl;
            return true;
        }
    }
}

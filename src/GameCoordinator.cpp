#include "GameCoordinator.hpp"

namespace makao
{
    GameCoordinator::GameCoordinator()
    {
        m_socket.setBlocking( false );
    }

    GameCoordinator::~GameCoordinator()
    {

    }

    bool GameCoordinator::bindSocket( const unsigned short t_port )
    {
        if ( m_socket.bind( t_port ) != sf::Socket::Done )
        {
            std::cerr << "Error binding socket!" << std::endl; 
            return false;
        }
        else
        {
            return true;
        }
    }

    void GameCoordinator::listen()
    {
        // std::cout << "Listening." << std::endl;

        Server temp;

        if ( receive( temp ) && temp.name != "**PING**" )
        {
            auto isPresent = std::find( m_activeServers.begin(), m_activeServers.end(), temp );

            if ( isPresent != m_activeServers.end() )
            {
                // std::cerr << isPresent->name << " tick!" << std::endl;
                *isPresent = temp;
                isPresent->restartClock();
            }
            else
            {
                std::cerr << temp.name << " connected." << std::endl;
                m_activeServers.push_back( temp );
            }
        }
        else if ( temp.name == "**PING**" )
        {
            std::cerr << temp.ip.toString() << " requested server list." << std::endl;
        }

        for ( auto server = m_activeServers.begin(); server != m_activeServers.end(); ++server )
        {
            if ( temp.name == "**PING**" )
            {
                sf::Packet tempPacket;
                tempPacket << *server;
                m_socket.send( tempPacket, temp.ip, temp.port );
            }

            if ( server->getElapsedTime() > 10.0 )
            {
                std::cerr << server->name << " disconnected." << std::endl;

                m_activeServers.erase( server );
                break;
            }
        }

        sf::sleep( sf::milliseconds( 100 ) );
    }

    bool GameCoordinator::receive( Server& t_server )
    {
        sf::IpAddress sender;
        sf::Packet    packet;
        ushort        port;

        // std::cerr << "Waiting for next packet..." << std::endl;

        if ( m_socket.receive( packet, sender, port ) != sf::Socket::Done )
        {
            // std::cerr << "Error receiving packet." << std::endl;
            return false;
        }
        else
        {
            packet >> t_server;

            return true;
        }
    }

    int GameCoordinator::getActiveServers() const
    {
        return m_activeServers.size();
    }
}

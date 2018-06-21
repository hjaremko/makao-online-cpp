#include "GameCoordinator.hpp"

namespace makao
{
    GameCoordinator::GameCoordinator( ncwindows::LogWindow* t_log ) : m_log( t_log )
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
        Server tmp;

        if ( receive( tmp ) && tmp.name != "**PING**" )
        {
            auto isPresent = std::find( m_activeServers.begin(), m_activeServers.end(), tmp );

            if ( isPresent != m_activeServers.end() )
            {
                // std::cerr << isPresent->name << " tick!" << std::endl;
                isPresent->restartClock();
            }
            else
            {
                // std::cerr << tmp.name << " connected." << std::endl;
                *m_log << std::string( tmp.name + " connected." );
                m_activeServers.push_back( tmp );
            }
        }
        else if ( tmp.name == "**PING**" )
        {
            // std::cerr << tmp.ip.toString() << " requested server list." << std::endl;
            *m_log << std::string( tmp.ip.toString() + " requested server list." );
        }

        for ( std::vector<Server>::iterator server = m_activeServers.begin(); 
                  server != m_activeServers.end(); ++server )
        {
            if ( tmp.name == "**PING**" )
            {
                ServerPacket tmpP;
                tmpP << *server;
                tmpP.send( &m_socket, tmp.ip, tmp.port );
            }

            if ( server->getElapsedTime() > 60.0 )
            {
                // std::cerr << server->name << " disconnected." << std::endl;
                *m_log << std::string( server->name + " disconnected." );

                m_activeServers.erase( server );
                break;
            }
        }

        sf::sleep( sf::milliseconds( 100 ) );
    }

    bool GameCoordinator::receive( auto& t_message )
    {
        sf::IpAddress sender;
        ServerPacket packet;
        unsigned short port;

        // std::cerr << "Waiting for next packet..." << std::endl;

        if ( m_socket.receive( packet, sender, port ) != sf::Socket::Done )
        {
            // std::cerr << "Error receiving packet." << std::endl;
            return false;
        }
        else
        {
            t_message.getFromPacket( packet );
            // std::cerr << "Received: \"" << message.name << "\" from " << sender.toString() << std::endl;

            return true;
        }
    }

    int GameCoordinator::getActiveServers() const
    {
        return m_activeServers.size();
    }
}
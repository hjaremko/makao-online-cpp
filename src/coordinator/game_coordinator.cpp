#include "coordinator/game_coordinator.hpp"

namespace makao
{
game_coordinator::game_coordinator()
{
    socket_.setBlocking( false );
}

game_coordinator::~game_coordinator() = default;

bool game_coordinator::bind_socket( const unsigned short port )
{
    if ( socket_.bind( port ) != sf::Socket::Done )
    {
        std::cerr << "Error binding socket!" << std::endl;
        return false;
    }

    return true;
}

void game_coordinator::listen()
{
    // std::cout << "Listening." << std::endl;

    server temp;
    constexpr std::string_view ping_signature{ "__PING__" };
    constexpr auto server_timeout{ 5 };

    if ( receive( temp ) && temp.name != ping_signature )
    {
        auto is_present = std::find( active_servers.begin(), active_servers.end(), temp );

        if ( is_present != active_servers.end() )
        {
            if ( is_present->get_elapsed_time() > server_timeout / 2.0f )
            {
                is_present->restart_clock();
                std::cerr << is_present->name << " tick!" << std::endl;
            }
        }
        else
        {
            std::cerr << temp.name << " connected." << std::endl;
            active_servers.push_back( temp );
        }
    }
    else if ( temp.name == ping_signature )
    {
        std::cerr << temp.ip.toString() << " requested server list." << std::endl;
    }

    for ( auto server = active_servers.begin(); server != active_servers.end(); ++server )
    {
        if ( temp.name == ping_signature )
        {
            sf::Packet temp_packet;
            temp_packet << *server;
            socket_.send( temp_packet, temp.ip, temp.port );
        }

        if ( server->get_elapsed_time() > server_timeout )
        {
            std::cerr << server->name << " disconnected." << std::endl;

            active_servers.erase( server );
            break;
        }
    }

    sf::sleep( sf::milliseconds( 100 ) );
}

bool game_coordinator::receive( server& t_server )
{
    sf::IpAddress sender;
    sf::Packet packet;
    ushort port{ 0 };

    // std::cerr << "Waiting for next packet..." << std::endl;

    if ( socket_.receive( packet, sender, port ) != sf::Socket::Done )
    {
        // std::cerr << "Error receiving packet." << std::endl;
        return false;
    }

    packet >> t_server;
    return true;
}

int game_coordinator::get_active_servers_count() const
{
    return active_servers.size();
}
} // namespace makao

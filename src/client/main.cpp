#include "makao-online/server/game.hpp"
#include "makao-online/server/server.hpp"

#include <SFML/Network.hpp>
#include <iostream>
//#include <sstream>
#include <vector>

using namespace makao;
using ushort = unsigned short;

int main( int /*argc*/, const char* /*argv*/[] )
{
    sf::UdpSocket socket;
    sf::TcpSocket tcp_socket;
    std::vector<server> servers;
    ushort client_port = 53000;

    // std::istringstream portStream( argv[ 1 ] );
    // portStream >> client_port;

    socket.setBlocking( false );

    while ( socket.bind( client_port ) != sf::Socket::Done )
    {
        std::cerr << "Error binding socket!" << std::endl;
        std::cerr << "Trying next port." << std::endl;
        client_port++;
    }

    server ping( sf::IpAddress::getLocalAddress(), client_port, "__PING__" );
    // server ping( sf::IpAddress::getPublicAddress(), client_port, "__PING__" );

    sf::Packet ping_packet;
    ping_packet << ping;

    // sf::IpAddress coordinator_ip_ = "narolnet.dynu.com";
    // sf::IpAddress coordinator_ip_ = "192.168.1.11";
    sf::IpAddress coordinator_ip = "127.0.0.1";

    std::cout << "===== Makao Online Makeshift Client =====" << std::endl
              << "[0] Refresh" << std::endl
              << "[q] Exit" << std::endl
              << "=========================================" << std::endl;

    char choice = 0;

    while ( ( choice = std::cin.get() ) != 'q' )
    {
        switch ( choice )
        {
        case '0':
        {
            sf::Clock clock;

            servers.clear();
            socket.send( ping_packet, coordinator_ip, 54000 );

            while ( static_cast<double>( clock.getElapsedTime().asSeconds() ) < 1.0 )
            {
                sf::IpAddress sender;
                sf::Packet packet;
                ushort port;

                if ( socket.receive( packet, sender, port ) == sf::Socket::Done )
                {
                    server tmp;

                    packet >> tmp;
                    servers.push_back( tmp );
                    clock.restart();

                    int i = 1;
                    for ( server& s : servers )
                    {
                        std::cout << "[" << i << "] " << s.get_info() << std::endl;
                        i++;
                    }
                }
                // else
                // {
                //     // std::cerr << "Error receiving packet." << std::endl;
                // }
            }

            break;
        }
        // case '1':
        // case -1:
        // break;
        default: // connect to the chosen server
        {
            auto server_index = static_cast<unsigned int>( choice - 49 );

            if ( server_index < servers.size() )
            {
                std::cout << "Game started!" << std::endl;

                sf::Packet game_packet;
                sf::Packet id_packet;
                int id = 0;

                std::cout << "Connecting to " << servers[ server_index ].ip.toString() << std::endl;
                tcp_socket.connect( servers[ server_index ].ip.toString(), 55001 );
                std::cout << "Waiting for id..." << std::endl;
                tcp_socket.receive( id_packet );
                id_packet >> id;
                std::cout << "You are player " << id << std::endl;

                while ( true )
                {
                    game game;

                    std::cout << "Waiting for game packet..." << std::endl;
                    tcp_socket.receive( game_packet );
                    std::cout << "Got the packet." << std::endl;
                    game_packet >> game;

                    game.print( id );

                    if ( game.get_player_state( id ) & player::turn )
                    {
                        int card_choice = 0;
                        sf::Packet choice_packet;

                        std::cout << "Your turn!" << std::endl;
                        std::cout << "Choose card >> ";

                        std::cin >> card_choice;
                        std::cin.clear();
                        std::cin.sync();

                        choice_packet << card_choice;
                        tcp_socket.send( choice_packet );
                    }
                }
            }

            std::cout << "game over!" << std::endl;

            break;
        }
        }
    }

    return 0;
}

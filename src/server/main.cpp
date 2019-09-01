#include "makao-online/server/game.hpp"
#include "makao-online/server/server.hpp"
#include "makao-online/network_interface.hpp"

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

using namespace makao;
using ushort = unsigned short;

template <class PacketType>
class background_sender
{
public:
    background_sender( PacketType& info, sf::UdpSocket& socket, sf::IpAddress& ip, ushort& port )
        : packet_info_( info ), socket_( socket ), recipient_address_( ip ), recipient_port_( port )
    {
    }

    void operator()() const
    {
        std::cout << "Sending server info to the coordinator..." << std::endl;

        while ( true )
        {
            sf::Packet packet;
            packet << packet_info_;
            socket_.send( packet, recipient_address_, recipient_port_ );
            sf::sleep( sf::milliseconds( 500 ) );
        }
    }

private:
    PacketType& packet_info_;
    sf::UdpSocket& socket_;
    sf::IpAddress& recipient_address_;
    ushort& recipient_port_;
};

int main( int /*argc*/, char const* /*argv*/[] )
{
    std::cout << "Makao Online server." << std::endl;

    sf::UdpSocket coordinator_socket;
    // sf::IpAddress coordinator_ip   = "192.168.1.11";
    sf::IpAddress coordinator_ip = "127.0.0.1";
    ushort coordinator_port = 54000;

    coordinator_socket.setBlocking( false );
    // server me( sf::IpAddress::getLocalAddress(), gcport, argv[ 1 ], 2 );
    server me( sf::IpAddress::getLocalAddress(), coordinator_port, "serwer", 2 );

    background_sender<server> bs( me, coordinator_socket, coordinator_ip, coordinator_port );
    std::thread sending_thread( bs );
    sending_thread.detach();

    // ----- The server -----
    sf::TcpListener listener;

    if ( listener.listen( 55001 ) == sf::Socket::Done )
    {
        while ( true )
        {
            std::cout << "Waiting for players..." << std::endl;

            auto game = std::make_unique<makao::game>( me.max_slots );

            sf::SocketSelector selector;
            selector.add( listener );

            if ( selector.wait() )
            // if ( selector.wait( sf::seconds( 60 ) ) )
            {
                for ( int i = 0; i < game->get_player_amount(); ++i )
                {
                    if ( selector.isReady( listener ) )
                    {
                        auto* tmp = new sf::TcpSocket;
                        //                        auto tmp = std::make_unique<sf::TcpSocket>();

                        if ( listener.accept( *tmp ) == sf::Socket::Done )
                        {
                            //                            game->get_player_socket( i, tmp.get() );
                            game->get_player_socket( i, tmp );
                            game->set_player_address( i, tmp->getRemoteAddress() );
                            game->send_id( i );

                            selector.add( *tmp );

                            me.taken_slots++;

                            std::cout << "New client connected: " << tmp->getRemoteAddress()
                                      << std::endl;
                        }
                        else
                        {
                            delete tmp;
                        }
                    }
                }
                //==========================================
                try
                {
                    std::cout << "Game started!" << std::endl;

                    int turn = 0;

                    while ( true )
                    {
                        std::cout << "Now turns: " << turn % game->get_player_amount() << std::endl;
                        game->set_turn( turn % game->get_player_amount() );
                        game->send_out();
                        int player_action = game->get_choice();

                        if ( game->is_valid( player_action ) )
                        {
                            game->make_turn( player_action );
                            turn++;
                        }
                        else if ( player_action < 0 )
                        {
                            game->draw_card( 1 );
                            turn++;
                        }
                        else
                        {
                            std::cout << "Wrong card!" << std::endl;
                        }
                    }

                    std::cout << "Game over!" << std::endl;
                }
                catch ( std::exception& e )
                {
                    std::cout << "Game over! Reason: " << e.what() << std::endl;
                }
                //==========================================

                me.taken_slots = 0;
            }
        }
    }

    return 0;
}

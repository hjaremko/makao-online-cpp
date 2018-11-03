#include <iostream>
#include <thread>

#include <SFML/Network.hpp>

#include "Server.hpp"
#include "Game.hpp"

using namespace makao;
using ushort = unsigned short;

template <class PacketType>
class BackgroundSending
{
    public:
        BackgroundSending( PacketType& t_info, sf::UdpSocket& t_socket, sf::IpAddress& t_ip, ushort& t_port ) :
                           m_packetInfo( t_info ), m_socket( t_socket ), m_recipientAddress( t_ip ), m_recipientPort( t_port ) {}

        void operator()() const
        {
            std::cout << "Sending server info to the coordinator..." << std::endl;

            while ( true )
            {
                sf::Packet packet;
                packet << m_packetInfo;
                m_socket.send( packet, m_recipientAddress, m_recipientPort );
                sf::sleep( sf::milliseconds( 500 ) );
            }
        }

    private:
        PacketType&     m_packetInfo;
        sf::UdpSocket&  m_socket;
        sf::IpAddress&  m_recipientAddress;
        ushort&         m_recipientPort;
};


int main( int argc, char const *argv[] )
{
    std::cout << "Makao Online Server." << std::endl;

    sf::UdpSocket gcSocket;
    // sf::IpAddress gcIp   = "192.168.1.11";
    sf::IpAddress gcIp   = "127.0.0.1";
    ushort        gcPort = 54000;

    gcSocket.setBlocking( false );
    // Server me( sf::IpAddress::getLocalAddress(), gcport, argv[ 1 ], 2 );
    Server me( sf::IpAddress::getLocalAddress(), gcPort, "serwer", 2 );

    BackgroundSending<Server> bs( me, gcSocket, gcIp, gcPort );
    std::thread sendingThread( bs );
    sendingThread.detach();
    

    // ----- The server -----
    sf::TcpListener listener;
    
    if ( listener.listen( 55001 ) == sf::Socket::Done )
    {
        while ( true )
        {
            std::cout << "Waiting for players..." << std::endl;

            auto game = std::make_unique<Game>( me.maxSlots );

            sf::SocketSelector selector;
            selector.add( listener );

            if ( selector.wait() )
            // if ( selector.wait( sf::seconds( 60 ) ) )
            {
                for ( int i = 0; i < game->getPlayerAmount(); ++i )
                {
                    if ( selector.isReady( listener ) )
                    {
                        sf::TcpSocket* tmp = new sf::TcpSocket;

                        if ( listener.accept( *tmp ) == sf::Socket::Done )
                        {
                            game->setPlayerSocket( i, tmp );
                            game->setPlayerAddress( i, tmp->getRemoteAddress() );
                            game->sendId( i );

                            selector.add( *tmp );

                            me.takenSlots++;

                            std::cout << "New client connected: " << tmp->getRemoteAddress() << std::endl;
                        }
                        else
                            delete tmp;
                    }
                }
                //==========================================
                try
                {
                    std::cout << "Game started!" << std::endl;

                    int turn = 0;

                    while ( true )
                    {
                        std::cout << "Now turns: " << turn % game->getPlayerAmount() << std::endl;
                        game->setTurn( turn % game->getPlayerAmount() );
                        game->sendOut();
                        int playerAction = game->getChoice();

                        if ( game->isValid( playerAction ) )
                        {
                            game->makeTurn( playerAction );
                            turn++;
                        }
                        else if ( playerAction < 0 )
                        {
                            game->drawCard( 1 );
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

                me.takenSlots = 0;
            }
        }
    }

    return 0;
}

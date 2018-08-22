#include <iostream>
#include <thread> 
#include <SFML/Network.hpp>
#include "Server.hpp"
#include "ServerPacket.hpp"
#include "Game.hpp"

using namespace makao;

class BackgroundSending
{
    public:
        BackgroundSending( Server& t_server, sf::UdpSocket& s, sf::IpAddress& i, unsigned short& po ) :
                           server( t_server ), socket( s ), ip( i ), port( po ) {}

        void operator()() const
        {
            std::cout << "Sending server info to the coordinator..." << std::endl;

            while ( true )
            {
                ServerPacket serverPacket;
                serverPacket << server;
                serverPacket.send( &socket, ip, port );
                sf::sleep( sf::milliseconds( 500 ) );
            }
        }

    private:
        sf::UdpSocket& socket;
        sf::IpAddress& ip;
        unsigned short& port;
        Server& server;
};


int main( int argc, char const *argv[] )
{
    std::cout << "Makao Online Server." << std::endl;

    sf::UdpSocket socket;
    sf::IpAddress gcIp = "127.0.0.1";
    unsigned short gcPort = 54000;

    // socket.bind( 56000 );
    socket.setBlocking( false );
    // Server me( recipient, port, argv[ 1 ], 2 );
    Server me( gcIp, gcPort, "serwer", 2 );

    BackgroundSending bs( me, socket, gcIp, gcPort );
    std::thread sendingThread( bs );
    sendingThread.detach();
    
    std::cout << "Waiting for players..." << std::endl;

    // ----- The server -----
    Game* game;

    sf::TcpListener listener;
    
    if ( listener.listen( 55001 ) == sf::Socket::Done )
    {
        while ( true )
        {
            std::cout << "Listening..." << std::endl;

            game = new Game( me.maxSlots );

            sf::SocketSelector selector;
            selector.add( listener );

            if ( selector.wait( sf::seconds( 60 ) ) )
            {
                for ( unsigned int i = 0; i < game->getPlayerAmount(); i++ )
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
                std::cout << "Game started!" << std::endl;

                int turn = 0;
                while ( true )
                {
                    std::cout << "Now turns: " << turn % game->getPlayerAmount() << std::endl;
                    game->setTurn( turn % game->getPlayerAmount() );
                    game->sendOut();
                    int choice = game->getChoice();

                    if ( game->isValid( choice ) )
                    {
                        turn++;
                    }
                    else
                    {
                        std::cout << "Wrong card!" << std::endl;
                    }
                }
                
                std::cout << "Game over!" << std::endl;
                me.takenSlots = 0;
                //==========================================
            }

            delete game;
        }
    }

    return 0;
}

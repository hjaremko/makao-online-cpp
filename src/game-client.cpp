#include <iostream>
#include <vector>
#include <sstream>
// #include <curses.h>
#include <conio.h>
#include <SFML/Network.hpp>
// #include "ncwindows"
#include "ServerPacket.hpp"
#include "Game.hpp"

using namespace makao;
// using namespace ncwindows;

int main( int argc, char const *argv[] )
{
    sf::UdpSocket socket;
    sf::TcpSocket tcpSocket;
    std::vector<Server> servers;
    // std::istringstream portStream( argv[ 1 ] );
    unsigned short clientPort = 53000;
    // portStream >> clientPort;

    socket.setBlocking( false );

    while ( socket.bind( clientPort ) != sf::Socket::Done )
    {
        std::cerr << "Error binding socket!" << std::endl;
        std::cerr << "Trying next port." << std::endl;
        clientPort++;
    }

    // Server ping( "localhost", clientPort, "**PING**" );
    Server ping( sf::IpAddress::getLocalAddress(), clientPort, "**PING**" );
    // Server ping( sf::IpAddress::getPublicAddress(), clientPort, "**PING**" );
    ServerPacket pingPacket;
    pingPacket << ping;
    // sf::IpAddress gameCoordinatorIp = "narolnet.dynu.com";
    // sf::IpAddress gameCoordinatorIp = "192.168.1.11";
    sf::IpAddress gameCoordinatorIp = "127.0.0.1";

    // initscr();
    // noecho();
    // Window::initColors();
    // start_color();

    // ContentWindow mainWindow( getmaxy( stdscr ), getmaxx( stdscr ), 0, 0 );
    // mainWindow.setBold( true );
    // mainWindow.newContent( 2, ( getmaxx( stdscr ) - 15 ) / 2, "..~~ MAKAO ~~..", mainWindow.Color::white );
    // mainWindow.draw();
    // mainWindow.setBold( false );

    // MenuWindow serverList( getmaxy( stdscr ) - 15, getmaxx( stdscr ) * 0.3 + 1, 7, ( ( getmaxx( stdscr ) - ( getmaxx( stdscr ) * 0.3 ) ) / 2 ) );
    // serverList << "refresh";
    // serverList << "exit";
    // serverList << "";
    std::cout << "===== Makao Online Makeshift Client =====" << std::endl
              << "[0] Refresh" << std::endl
              << "[q] Exit" << std::endl
              << "=========================================" << std::endl;

    char choice = 0;
    // while ( ( choice = serverList.selectLoop( 'q' ) ) == 0 )
    while ( ( choice = getch() ) != 'q' )
    {
        switch ( choice )
        {
            case '0':
            {
                servers.clear();
                // serverList.clear( 2 );
                pingPacket.send( &socket, gameCoordinatorIp, 54000 );
                sf::Clock clock;
                
                while ( clock.getElapsedTime().asSeconds() < 1.0 )
                {
                    sf::IpAddress sender;
                    ServerPacket packet;
                    unsigned short port;

                    if ( socket.receive( packet, sender, port ) == sf::Socket::Done )
                    {
                        Server tmp;
                        packet >> tmp;
                        servers.push_back( tmp );
                        clock.restart();

                        // serverList.newMessage( tmp.getInfo() );
                        int i = 1;
                        for ( Server& s : servers )
                        {
                            std::cout << "[" << i << "] " << s.getInfo() << std::endl;
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
            default:  //connect to the chosen server
            {
                // serverList.hide();
                unsigned serverIndex = choice - 49;

                if ( serverIndex < servers.size() )
                {
                    std::cout << "Game started!" << std::endl;
                    
                    sf::Packet gamePacket;
                    sf::Packet idPacket;
                    int id;

                    std::cout << "Connecting to " << servers[ serverIndex ].ip.toString() << std::endl;
                    tcpSocket.connect( servers[ serverIndex ].ip.toString(), 55001 );
                    std::cout << "Waiting for id..." << std::endl;
                    tcpSocket.receive( idPacket );
                    idPacket >> id;
                    std::cout << "You are player " << id << std::endl;

                    while ( true )
                    {
                        Game game;
                        std::cout << "Waiting for game packet..." << std::endl;
                        tcpSocket.receive( gamePacket );
                        std::cout << "Got the packet." << std::endl;
                        gamePacket >> game;

                        game.print( id );

                        if ( game.getPlayerState( id ) & Player::Turn )
                        {
                            int cardChoice = 0;
                            sf::Packet choicePacket;

                            std::cout << "Your turn!" << std::endl;
                            std::cout << "Choose card >> ";
                            std::cin >> cardChoice;
                            std::cin.clear();
                            std::cin.sync();
                            choicePacket << cardChoice;
                            tcpSocket.send( choicePacket );
                        }
                    }
                }

                std::cout << "Game over!" << std::endl;
                
                break;
            }
        }
    }

    // getch();
    // endwin();
    
    return 0;
}

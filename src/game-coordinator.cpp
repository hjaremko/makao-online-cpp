#include <iostream>
#include <algorithm>
#include <thread>
#include <string>
#include <sstream>
#include <SFML/Network.hpp>
#include "GameCoordinator.hpp"

using namespace makao;

int main( int argc, char** argv )
{
    std::cout << "Makao Online Game Coordinator." << std::endl;

    int activeServers = 0;
    GameCoordinator gameCoordinator;
    gameCoordinator.bindSocket( 54000 );


    while ( true )
    {
        gameCoordinator.listen();
        activeServers = gameCoordinator.getActiveServers();
    }

    return 0;
}
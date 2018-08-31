#include <iostream>

#include <SFML/Network.hpp>

#include "GameCoordinator.hpp"

using namespace makao;

int main( int argc, char const *argv[] )
{
    std::cout << "Makao Online Game Coordinator." << std::endl;

    GameCoordinator gameCoordinator;
    gameCoordinator.bindSocket( 54000 );

    while ( true )
    {
        gameCoordinator.listen();
    }

    return 0;
}

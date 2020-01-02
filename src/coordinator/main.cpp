//TODO: rewrite
#include "coordinator/game_coordinator.hpp"

#include <iostream>

using namespace makao;

int main( int /*argc*/, char const* /*argv*/[] )
{
    std::cout << "Makao Online game Coordinator." << std::endl;

    game_coordinator coordinator;
    coordinator.bind_socket( 54000 );

    while ( true )
    {
        coordinator.listen();
    }

    return 0;
}

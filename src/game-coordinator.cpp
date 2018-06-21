#include <iostream>
#include <algorithm>
#include <thread>
#include <string>
#include <sstream>
#include <curses.h>
#include <SFML/Network.hpp>
#include "ncwindows"
#include "GameCoordinator.hpp"

using namespace ncwindows;
using namespace makao;


int main( int argc, char** argv )
{
    initscr();

    ContentWindow head( 3, getmaxx( stdscr ), 0, 0 );
    LogWindow log( getmaxy( stdscr ) - 3, getmaxx( stdscr ), 3, 0 );
    int activeServers = 0;
    head.newContent( 1, 2, "Active servers: " );
    head.newContent( 1, 18, &activeServers );

    GameCoordinator gameCoordinator( &log );
    gameCoordinator.bindSocket( 54000 );

    while ( true )
    {
        gameCoordinator.listen();
        activeServers = gameCoordinator.getActiveServers();
        head.draw();
    }

    getch();
    endwin();
    return 0;
}
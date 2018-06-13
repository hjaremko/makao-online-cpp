#include <time.h>
#include <iostream>
#include "LogWindow.hpp"

const std::string currentDateTime()
{
    time_t     now = time( 0 );
    struct tm  tstruct;
    char       buf[ 80 ];
    tstruct = *localtime( &now );
    strftime( buf, sizeof( buf ), "[ %X ]", &tstruct );

    return buf;
}

LogWindow& operator<<( LogWindow& log, const int t_message )
{
    log.newMessage( std::to_string( t_message ) );

    return log;
}

LogWindow& operator<<( LogWindow& log, const std::string& t_message )
{
    log.newMessage( t_message );

    return log;
}

LogWindow::LogWindow( int t_height, int t_width, int t_starty, int t_startx ) 
                    : ContentWindow( t_height, t_width, t_starty, t_startx )
{
    draw();
}

void LogWindow::newMessage( const std::string& t_message )
{
    auto tmp = new std::string( currentDateTime() + " " + t_message );
    newContent( m_contents.size() + 1, 1, tmp );

    if ( m_contents.size() > m_maxContents )
    {
        scroll( 1 );
    }

    draw();
}
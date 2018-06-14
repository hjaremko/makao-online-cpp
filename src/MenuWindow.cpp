#include "MenuWindow.hpp"
#include <iostream>

MenuWindow::MenuWindow( int t_height, int t_width, int t_starty, int t_startx ) 
                      : LogWindow( t_height, t_width, t_starty, t_startx )
{

}

void MenuWindow::draw()
{
    refresh();
    wclear( m_window );
    
    int current = 0;
    for ( const auto& content : m_contents )
    {
        if ( current == m_selected )
        {
            setReverse( true );
        }

        setColor( content.get()->getColor(), true );
        mvwprintw( m_window, content.get()->y() + m_yModifier,
                             content.get()->x() + m_xModifier,
                       "%s", content.get()->get().c_str() );
        setColor( content.get()->getColor(), false );

        current++;
        wattrset( m_window, A_NORMAL );
    }

    drawBorder();
    wrefresh( m_window );
}

void MenuWindow::newMessage( const std::string& t_message )
{
    auto tmp = new std::string( t_message );
    newContent( m_contents.size() + 1, ( getmaxx( m_window ) - t_message.length() ) / 2, tmp );

    draw();
}

void MenuWindow::selectDown()
{
    m_selected++;

    if ( m_selected >= m_contents.size() )
    {
        m_selected = m_contents.size() - 1;
    }

    if ( m_selected >= m_maxContents - m_yModifier )
    {
        scroll( 1 );
    }

    draw();
}

void MenuWindow::selectUp()
{
    m_selected--;

    if ( m_selected < 0 )
    {
        m_selected = 0;
    }

    if ( m_selected < -m_yModifier )
    {
        scroll( -1 );
    }

    draw();
}

int MenuWindow::selectLoop( char t_end )
{
    char ch;
    while ( ( ch = getch() ) != t_end )
    {
        std::cerr << ch;
        switch ( ch )
        {   
            case 's':
                selectDown();
                break;
            case 'w':
                selectUp();
                break;
            case '\n':
                return m_selected;
                break;
        }

    }    
}
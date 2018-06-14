#include "Window.hpp"

Window::Window( int t_height, int t_width, int t_starty, int t_startx ) 
                : m_height( t_height ), m_width( t_width ), 
                  m_starty( t_starty ), m_startx( t_startx )
{
    m_window = createWindow( m_height, m_width, m_starty, m_startx );
}

Window::~Window()
{
    wclear( m_window );
    wrefresh( m_window );
    destroyWindow( m_window );
} //dtor

WINDOW* Window::createWindow( int t_height, int t_width, int t_starty, int t_startx )
{   
    WINDOW *localWindow;

    localWindow = newwin( t_height, t_width, t_starty, t_startx );
    wrefresh( localWindow );

    return localWindow;
}

void Window::destroyWindow( WINDOW* t_localWindow )
{   
    wborder( t_localWindow, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
    wrefresh( t_localWindow );
    delwin( t_localWindow );
}

void Window::printCentered( WINDOW* t_window, int t_y, std::string t_message ) const
{
    mvwprintw( t_window, t_y,
             ( getmaxx( t_window ) - t_message.length() ) / 2, 
             t_message.c_str() );
    wrefresh( t_window );
}

void Window::draw()
{
    refresh();
    wclear( m_window );
    wprintw( m_window, "\n" );

    drawBorder();
    wrefresh( m_window );
}

void Window::drawBorder()
{
    box( m_window, 0 , 0 );
    wrefresh( m_window );
}

WINDOW* Window::getWindow() const
{
    return m_window;
}
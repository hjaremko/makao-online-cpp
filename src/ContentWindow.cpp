#include "ContentWindow.hpp"

namespace ncwindows
{
    ContentWindow::ContentWindow( int t_height, int t_width, int t_starty, int t_startx )
                                : Window( t_height, t_width, t_starty, t_startx )
    {
        draw();
        m_maxContents = m_height - 2;
    }

    void ContentWindow::draw()
    {
        refresh();
        wclear( m_window );

        for ( const auto& content : m_contents )
        {
            setColor( content.get()->getColor(), true );
            mvwprintw( m_window, content.get()->y() + m_yModifier,
                                 content.get()->x() + m_xModifier,
                           "%s", content.get()->get().c_str() );
            setColor( content.get()->getColor(), false );
        }

        drawBorder();
        wrefresh( m_window );
    }

    void ContentWindow::setBold( bool mode )
    {
        if ( mode )
            wattron( m_window, A_BOLD );
        else
            wattroff( m_window, A_BOLD );
    }

    void ContentWindow::setReverse( bool mode )
    {
        if ( mode )
            wattron( m_window, A_REVERSE );
        else
            wattroff( m_window, A_REVERSE );
    }
    void ContentWindow::setColor( int color, bool mode )
    {
        if ( mode )
            wattron( m_window, COLOR_PAIR( color ) );
        else
            wattroff( m_window, COLOR_PAIR( color ) );
    }

    void ContentWindow::scroll( int t_lines )
    {
        m_yModifier -= t_lines;
    }
}

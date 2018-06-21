#include "InputForm.hpp"

namespace ncwindows
{
    InputForm::InputForm( int t_height, int t_width, int t_starty, int t_startx )
                        : Window( t_height, t_width, t_starty, t_startx )
    {

    }

    std::string InputForm::get()
    {
        draw();
        wmove( m_window, 1, 1 );
        echo();
        
        char str[ 150 ];
        wgetstr( m_window, str );

        noecho();

        return str;
    }
}
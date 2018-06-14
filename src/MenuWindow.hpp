#pragma once

#include <curses.h>
#include <string>
#include "LogWindow.hpp"

class MenuWindow : public LogWindow
{
    public:
        MenuWindow( int, int, int, int );
        void draw();
        void newMessage( const std::string& );
        void selectDown();
        void selectUp();
        int selectLoop( char );

    private:
        int m_selected{ 0 };
};
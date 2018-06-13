#pragma once

#include <string>
#include <vector>
#include <curses.h>

class Window
{
    public:
        Window( int, int, int, int );
        virtual ~Window();

        WINDOW* getWindow() const;
        void draw();
        void drawBorder();

    protected:
        WINDOW* createWindow( int, int, int, int );
        void destroyWindow( WINDOW* );
        void printCentered( WINDOW*, int, std::string ) const;

        WINDOW* m_window{ nullptr };
        int m_height{ 0 };
        int m_width{ 0 };
        int m_starty{ 0 };
        int m_startx{ 0 };
};

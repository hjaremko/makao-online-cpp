#pragma once

#include <string>
#include <curses.h>
#include "ContentWindow.hpp"

class LogWindow : public ContentWindow
{
    public:
        LogWindow( int, int, int, int );
        virtual ~LogWindow() {}

        friend LogWindow& operator<<( LogWindow&, const std::string& );
        friend LogWindow& operator<<( LogWindow&, const int );
        
    private:
        virtual void newMessage( const std::string& );
};


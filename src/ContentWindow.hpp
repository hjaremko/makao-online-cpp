#pragma once

#include <string>
#include <memory>
#include <curses.h>
#include "Window.hpp"
#include "Content.hpp"

class ContentWindow : public Window
{
    public:
        ContentWindow( int, int, int, int );
        virtual ~ContentWindow() {};

        template <class T>
        void newContent( const int, const int, T*, const int = 7 );

        void draw();
        void setBold( bool );
        void setColor( int, bool );
        void scroll( int );

    protected:
        std::vector<std::unique_ptr<ContentInterface>> m_contents;
        int m_maxContents{ 0 };
        int m_yModifier{ 0 };
        int m_xModifier{ 0 };
};

template <class T>
void ContentWindow::newContent( const int t_y, const int t_x,
                                T* t_content, const int t_color )
{
    m_contents.push_back( std::make_unique<Content<T>>( t_y, t_x, t_content, t_color ) );
}
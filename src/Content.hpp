#pragma once

#include <string>

namespace ncwindows
{
    class ContentInterface
    {
        public:
            ContentInterface( int t_y, int t_x, int t_color )
                              : m_y( t_y ), m_x( t_x ), m_color( t_color ) {}
            virtual ~ContentInterface() {}

            virtual std::string get() {}

            virtual int x() const
            {
                return m_x;
            }

            virtual int y() const
            {
                return m_y;
            }

            virtual int getColor() const
            {
                return m_color;
            }

            virtual void move( const int y, const int x )
            {
                m_y += y;
                m_x += x;
            }

            virtual void moveUp( const int n )
            {
                m_y -= n;
            }

            virtual void moveDown( const int n )
            {
                m_y += n;
            }

            virtual void moveRight( const int n )
            {
                m_x += n;
            }

            virtual void moveLeft( const int n )
            {
                m_x -= n;
            }

        protected:
            int m_y{ 0 };
            int m_x{ 0 };
            int m_color{ 7 };
    };

    template <class T>
    class Content : public ContentInterface
    {
        public:
            Content<T>( int, int, T*, int = 7 );
            virtual ~Content() {}

            std::string get();
            T* getPointer() const;

        private:
            T* m_content{ nullptr };
    };

    template <class T>
    Content<T>::Content( int t_y, int t_x, T* t_content, int t_color )
                         : m_content( t_content ), ContentInterface( t_y, t_x, t_color )
    {

    }
}

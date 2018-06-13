#include "Content.hpp"

template <>
std::string Content<int>::get()
{
    return std::to_string( *m_content );
}

template <>
std::string Content<std::string>::get()
{
    return *m_content;
}

template <>
std::string Content<char const>::get()
{
    return std::string( m_content );
}

template <>
std::string Content<char const*>::get()
{
    return std::string( *m_content );
}

template <class T>
T* Content<T>::getPointer() const
{
    return m_content;
}
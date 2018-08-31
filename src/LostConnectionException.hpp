// #include <iostream>
#include <exception>

class LostConnectionException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Lost connection!";
    }
};
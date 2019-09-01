#pragma once

#include <exception>
#include <sstream>
#include <string>

namespace makao
{

class lost_connection : public std::exception
{
public:
    explicit lost_connection( const int id )
    {
        std::stringstream s;
        s << "Lost connection to player " << id << "!";
        whats = s.str();
    }

    [[nodiscard]] virtual const char* what() const noexcept
    {
        return whats.c_str();
    }

private:
    std::string whats{ "" };
};

} // namespace makao

#ifndef MAKAO_ONLINE_LOST_CONNECTION_EXCEPTION_HPP
#define MAKAO_ONLINE_LOST_CONNECTION_EXCEPTION_HPP

#include <exception>
#include <sstream>
#include <string>

namespace makao
{
class lost_connection_exception : public std::exception
{
public:
    explicit lost_connection_exception( const int id )
    {
        std::stringstream s;
        s << "Lost connection to player " << id << "!";
        whats = s.str();
    }

    [[nodiscard]] const char* what() const noexcept override
    {
        return whats.c_str();
    }

private:
    std::string whats;
};
} // namespace makao
#endif

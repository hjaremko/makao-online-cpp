#include "makao-online/server/old_server.hpp"

#include <utility>

using ushort = unsigned short;

namespace makao
{
server::server() = default;

server::server( sf::IpAddress addr, ushort p, std::string n, int slots )
    : ip( addr ), port( p ), name( std::move( n ) ), max_slots( slots ), taken_slots( 0 )
{
}

bool server::operator==( const server& rhs ) const
{
    return ( name == rhs.name && ip == rhs.ip );
}

void server::set_ip( const std::string& address )
{
    ip = address;
}

std::string server::get_info() const
{
    return ( name + " " + std::to_string( taken_slots ) + "/" + std::to_string( max_slots ) );
}

float server::get_elapsed_time() const
{
    return timeout_.getElapsedTime().asSeconds();
}

void server::restart_clock()
{
    timeout_.restart();
}
} // namespace makao

sf::Packet& operator<<( sf::Packet& lhs, const makao::server& rhs )
{
    // std::cout << "Sending: " << rhs.get_info() << std::endl;

    return lhs << rhs.ip.toString() << rhs.port << rhs.name << rhs.taken_slots << rhs.max_slots;
}

sf::Packet& operator>>( sf::Packet& lhs, makao::server& rhs )
{

    std::string tmpIp;

    lhs >> tmpIp >> rhs.port >> rhs.name >> rhs.taken_slots >> rhs.max_slots;
    rhs.ip = tmpIp;

    // std::cout << "Receiving: " << t_server.get_info() << std::endl;

    return lhs;
}

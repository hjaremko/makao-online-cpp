#ifndef MAKAO_ONLINE_SFML_SERVER_CONFIG_HPP
#define MAKAO_ONLINE_SFML_SERVER_CONFIG_HPP

//TODO: separate sfml from the server config

#include <SFML/Network.hpp>
#include <string>

namespace makao
{
struct server_config
{
    using port_t = unsigned short;

    friend sf::Packet& operator<<( sf::Packet& lhs, const server_config& rhs )
    {
        // std::cout << "Sending: " << rhs.get_info() << std::endl;
        return lhs << rhs.ip.toString() << rhs.port << rhs.name
                   << rhs.taken_slots << rhs.max_slots;
    }

    std::string name;
    sf::IpAddress ip{ "127.0.0.1" };
    port_t port{ 0 };
    int taken_slots{ 0 };
    int max_slots{ 0 };
};
} // namespace makao
#endif

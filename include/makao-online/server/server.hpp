#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <string>

using ushort = unsigned short;

namespace makao
{
class server
{
public:
    server();
    server( sf::IpAddress, unsigned short, std::string, int = 4 );

    bool operator==( const server& ) const;
    [[nodiscard]] std::string get_info() const;
    [[nodiscard]] float get_elapsed_time() const;
    void set_ip( const std::string& );
    void restart_clock();

    sf::IpAddress ip;
    ushort port{ 0 };
    std::string name{ "blank" };
    int max_slots{ 0 };
    int taken_slots{ 0 };

private:
    sf::Clock timeout_;
};
} // namespace makao

sf::Packet& operator<<( sf::Packet&, const makao::server& );
sf::Packet& operator>>( sf::Packet&, makao::server& );

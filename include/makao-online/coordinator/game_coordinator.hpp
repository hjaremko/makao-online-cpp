#pragma once

#include "makao-online/server/server.hpp"

#include <SFML/Network.hpp>
#include <iostream>
#include <vector>

namespace makao
{
class game_coordinator
{
public:
    game_coordinator();
    ~game_coordinator();

    void listen();
    [[nodiscard]] int get_active_servers_count() const;
    bool bind_socket( unsigned short );
    bool receive( server& );

private:
    sf::UdpSocket socket_;
    sf::SocketSelector selector_;
    std::vector<sf::UdpSocket*> sockets_;
    std::vector<server> active_servers;
};
} // namespace makao

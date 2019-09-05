#pragma once

#include <SFML/Network.hpp>
#include "makao-online/server/server.hpp"
#include <vector>

namespace makao
{
class client
{
public:
    // bool bind( short int port )
    //{
    //    return false;
    //};

private:
    std::vector<server> servers_;
    sf::UdpSocket socket_;
    sf::IpAddress coordinator_ip_;
    server ping_;
    // Makao::ContentWindow mainWindow;
};
} // namespace makao

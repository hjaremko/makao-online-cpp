#pragma once

#include "makao-online/card.hpp"
#include "makao-online/deck.hpp"

#include <SFML/Network.hpp>
#include <iostream>

namespace makao
{
class game;

class player
{
public:
    friend game;

    enum state
    {
        clear = 0,
        turn = 1,
        take = 2,
        stop = 4,
        jack = 8,
        ace = 16
    };

    player();
    explicit player( int );
    void take_card( const std::shared_ptr<card>& );
    void set_socket( sf::TcpSocket* );
    void set_address( sf::IpAddress );
    void set_state( state );
    void add_state( state );
    void print_states() const;
    [[nodiscard]] std::shared_ptr<card> throw_card( int );
    [[nodiscard]] std::shared_ptr<deck> get_hand() const;
    [[nodiscard]] sf::TcpSocket* get_socket() const;
    [[nodiscard]] sf::IpAddress get_address() const;
    [[nodiscard]] state get_state() const;
    [[nodiscard]] int get_id() const;

    friend sf::Packet& operator<<( sf::Packet& packet, const player* player )
    {
        return packet << player->hand_.get() << player->state_flags_;
    }

    friend sf::Packet& operator>>( sf::Packet& packet, player& player )
    {
        int tmp{ 0 };
        packet >> *player.hand_ >> tmp;
        player.state_flags_ = static_cast<state>( tmp );
        return packet;
    }

private:
    std::shared_ptr<deck> hand_{ std::make_shared<deck>() };
    state state_flags_{ clear };
    sf::TcpSocket* socket_{ nullptr };
    sf::IpAddress address_;
    int id_{ 0 };
};
} // namespace makao

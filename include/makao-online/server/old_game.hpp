#pragma once

#include "makao-online/deck.hpp"
#include "old_player.hpp"

#include <iostream>
#include <sstream>
#include <vector>

namespace makao
{
class game
{
public:
    game();
    explicit game( int );

    void print() const;
    void print( unsigned int ) const;
    void send_out() const;
    void send_id( unsigned int );
    void set_turn( unsigned int );
    void get_player_socket( int index, sf::TcpSocket* );
    void set_player_address( int index, sf::IpAddress );
    void make_turn( int );
    void draw_card( int );
    [[nodiscard]] int get_choice() const;
    [[nodiscard]] int get_player_amount() const;
    [[nodiscard]] player::state get_player_state( int ) const;
    [[nodiscard]] auto get_turn_player() const;
    [[nodiscard]] bool is_valid( unsigned int ) const;
    void shuffle_decks();

    friend sf::Packet& operator<<( sf::Packet& packet, const game& game )
    {
        packet << static_cast<int>( game.players_.size() );

        for ( const auto& player : game.players_ )
        {
            packet << player.get();
        }

        return packet << game.drawing_deck_.get() << game.playing_deck_.get();
    }

    friend sf::Packet& operator>>( sf::Packet& packet, game& game )
    {
        int size;
        packet >> size;

        for ( int i = 0; i < size; ++i )
        {
            auto tmp = std::make_shared<player>();
            packet >> *tmp;
            game.players_.push_back( tmp );
        }

        return packet >> *game.drawing_deck_ >> *game.playing_deck_;
    }

private:
    void make_players( int );
    void make_decks();

    std::vector<std::shared_ptr<player>> players_;
    std::shared_ptr<deck> drawing_deck_{ std::make_shared<deck>() };
    std::shared_ptr<deck> playing_deck_{ std::make_shared<deck>() };
};
} // namespace makao

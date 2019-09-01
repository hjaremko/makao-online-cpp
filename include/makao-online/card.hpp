#pragma once

#include <SFML/Network.hpp>
#include <array>
#include <iostream>
#include <string>

namespace makao
{
class card
{
public:
    enum suit
    {
        hearts = 0,
        diamonds,
        clubs,
        spades
    };

    enum figure
    {
        ace = 0,
        two,
        three,
        four,
        five,
        six,
        seven,
        eight,
        nine,
        ten,
        jack,
        queen,
        king
    };

    card();
    card( suit, figure, bool = false );
    [[nodiscard]] std::string to_string() const;

    bool operator==( const card& c )
    {
        return suit_ == c.suit_ || figure_ == c.figure_;
    }

    friend sf::Packet& operator<<( sf::Packet& packet, const card& c )
    {
        return packet << c.suit_ << c.figure_;
    }

    friend sf::Packet& operator>>( sf::Packet& packet, card& card )
    {
        int tmp_suit{ 0 };
        int tmp_fig{ 0 };
        packet >> tmp_suit >> tmp_fig;
        card.suit_ = static_cast<card::suit>( tmp_suit );
        card.figure_ = static_cast<card::figure>( tmp_fig );

        return packet;
    }

private:
    suit suit_{ hearts };
    figure figure_{ ace };
    bool is_special_{ false };
};
} // namespace makao

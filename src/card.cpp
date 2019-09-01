#include "makao-online/card.hpp"

namespace makao
{
card::card() = default;

card::card( suit s, figure fig, bool special ) : suit_( s ), figure_( fig ), is_special_( special )
{
}

std::string card::to_string() const
{
    std::array<std::string, 4> suits = { "hearts", "diamonds", "clubs", "spades" };

    std::array<std::string, 13> figures = { "ace",  "two",   "three", "four", "five",
                                            "six",  "seven", "eight", "nine", "ten",
                                            "jack", "queen", "king" };

    std::string temp = figures[ figure_ ] + "_" + suits[ suit_ ];
    return temp;
}
} // namespace makao

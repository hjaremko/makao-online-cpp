#include "makao-online/card.hpp"

#include <unordered_map>

namespace makao
{
const std::unordered_map<card::suit, std::string> suit_to_str { { card::suit::hearts, "hearts" },
                                                                { card::suit::diamonds,
                                                                  "diamonds" },
                                                                { card::suit::clubs, "clubs" },
                                                                { card::suit::spades, "spades" } };

const std::unordered_map<card::figure, std::string> figure_to_str {
    { card::figure::ace, "ace" },     { card::figure::two, "two" },
    { card::figure::three, "three" }, { card::figure::four, "four" },
    { card::figure::five, "five" },   { card::figure::six, "six" },
    { card::figure::seven, "seven" }, { card::figure::eight, "eight" },
    { card::figure::nine, "nine" },   { card::figure::ten, "ten" },
    { card::figure::jack, "jack" },   { card::figure::queen, "queen" },
    { card::figure::king, "king" }
};

card::card( suit s, figure fig ) : suit_( s ), figure_( fig )
{
}

std::string card::to_string() const
{
    return { figure_to_str.at( figure_ ) + "_" + suit_to_str.at( suit_ ) };
}
} // namespace makao

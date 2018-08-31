#include "Card.hpp"

namespace makao
{
    Card::Card()
    {

    }

    Card::Card( Suit t_suit, Figure t_figure, bool t_special )
              : m_suit( t_suit ), m_figure( t_figure ), m_special( t_special )
    {

    }

    std::string Card::str() const
    {
        std::array<std::string, 4> suits = {  "hearts",
                                              "diamonds",
                                              "clubs",
                                              "spades" };

        std::array<std::string, 13> figures =   {
                                                    "ace",
                                                    "two",
                                                    "three",
                                                    "four",
                                                    "five",
                                                    "six",
                                                    "seven",
                                                    "eight",
                                                    "nine",
                                                    "ten",
                                                    "jack",
                                                    "queen",
                                                    "king"
                                                };

        std::string temp = figures[ m_figure ] + "_" + suits[ m_suit ];

        return temp;
    }
}

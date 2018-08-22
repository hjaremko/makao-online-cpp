#pragma once

#include <string>
#include <array>
#include <iostream>

namespace makao
{
    class Card
    {
        public:
            enum Suit
            {
                hearts = 0,
                diamonds,
                clubs,
                spades
            };

            enum Figure
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

            Card() {}
            Card( Suit t_suit, Figure t_figure, bool t_special = false ) 
            : m_suit( t_suit ), m_figure( t_figure ), m_special( t_special )
            {
            };

            std::string str() const
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

            bool operator==( const Card& t_card )
            {
                return ( ( m_suit == t_card.m_suit ) || ( m_figure == t_card.m_figure ) );
            }

            friend sf::Packet& operator<<( sf::Packet& packet, const Card* card )
            {
                return packet << card->m_suit << card->m_figure;
            }

            friend sf::Packet& operator>>( sf::Packet& packet, Card& card )
            {
                int tmpSuit;
                int tmpFig;
                packet >> tmpSuit >> tmpFig;
                card.m_suit = static_cast<Card::Suit>( tmpSuit );
                card.m_figure = static_cast<Card::Figure>( tmpFig );

                return packet;
            }

        private:
            Suit m_suit;
            Figure m_figure;
            bool m_special = false;
    };
}

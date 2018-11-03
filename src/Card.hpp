#pragma once

#include <string>
#include <array>
#include <iostream>

#include <SFML/Network.hpp>

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

            Card();
            Card( Suit, Figure, bool = false );
            std::string str() const;

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
            Suit   m_suit{ hearts };
            Figure m_figure{ ace };
            bool   m_special{ false };
    };
}

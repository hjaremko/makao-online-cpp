#pragma once

#include <SFML/Network.hpp>

#include "Card.hpp"
#include <vector>
#include <iostream>

namespace makao
{
    class Player;

    class Deck
    {
        public:
            Deck() {};

            void make( int );
            void shuffle();
            void dealOut( int, Player* );
            void push( Card* );
            void print() const;
            Card* pop();
            Card* remove( int );
            Card* peek() const;
            Card* at( int i ) const { return  m_cards.at( i ); }
            int getSize() const { return m_cards.size(); }


            friend sf::Packet& operator<<( sf::Packet& packet, const Deck* deck )
            {
                packet << deck->m_cards.size();

                for ( std::vector<Card*>::const_iterator it = deck->m_cards.begin(); it != deck->m_cards.end(); ++it )
                {
                    packet << *it;
                }

                return packet;
            }

            friend sf::Packet& operator>>( sf::Packet& packet, Deck* deck )
            {
                int size;
                packet >> size;

                // std::cout << "Taking out " << size << " cards..." << std::endl;

                for ( int i = 0; i < size; ++i )
                {
                    Card* tmp = new Card();
                    packet >> *tmp;
                    deck->m_cards.push_back( tmp );
                }

                return packet;
            }

        private:
            std::vector<Card*> m_cards;
    };
}

#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include <SFML/Network.hpp>

#include "Card.hpp"

namespace makao
{
    class Player;

    class Deck
    {
        public:
            Deck() {};

            void make( int );
            void shuffle();
            void dealOut( int, std::shared_ptr<Player> );
            void push( std::shared_ptr<Card> );
            void print() const;
            std::shared_ptr<Card> pop();
            std::shared_ptr<Card> remove( int );
            std::shared_ptr<Card> peek() const;
            std::shared_ptr<Card> at( int i ) const;
            int getSize() const;

            friend sf::Packet& operator<<( sf::Packet& packet, const Deck* deck )
            {
                packet << deck->m_cards.size();

                for ( auto it = deck->m_cards.begin(); it != deck->m_cards.end(); ++it )
                {
                    packet << ( *it ).get();
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
                    std::shared_ptr<Card> tmp = std::make_shared<Card>();
                    packet >> *tmp;
                    deck->m_cards.push_back( tmp );
                }

                return packet;
            }

        private:
            std::vector<std::shared_ptr<Card>> m_cards;
    };
}

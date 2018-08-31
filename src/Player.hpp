#pragma once

#include <iostream>

#include <SFML/Network.hpp>

#include "Card.hpp"
#include "Deck.hpp"

namespace makao
{
    class Game;

    class Player
    {
        public:
            friend Game;

            enum State
            {
                Clear = 0,
                Turn  = 1,
                Take  = 2,
                Stop  = 4,
                Jack  = 8,
                Ace   = 16
            };

            Player();
            Player( int );
            ~Player();
            void take( std::shared_ptr<Card> );
            void setSocket( sf::TcpSocket* );
            void setAddress( sf::IpAddress );
            void setState( State );
            void addState( State );
            void printStates() const;
            std::shared_ptr<Card> throwCard( int );
            std::shared_ptr<Deck> getHand() const;
            sf::TcpSocket* getSocket() const;
            sf::IpAddress getAddress() const;
            State getState() const;
            int getId() const;

            friend sf::Packet& operator<<( sf::Packet& packet, const Player* player )
            {
                return packet << player->m_hand.get() << player->m_stateFlags;
            }

            friend sf::Packet& operator>>( sf::Packet& packet, Player& player )
            {
                int tmp;
                packet >> player.m_hand.get() >> tmp;
                player.m_stateFlags = static_cast<State>( tmp );
                return packet;
            }

        private:
            std::shared_ptr<Deck> m_hand{ std::make_shared<Deck>() };
            State m_stateFlags{ Clear };
            sf::TcpSocket* m_socket;
            sf::IpAddress m_address;
            int m_id{ 0 };
    };
}

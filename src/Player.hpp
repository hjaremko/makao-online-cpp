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
            Player() {}
            Player( int t_id ) : m_id( t_id ) {}
            ~Player() { delete m_hand; }

            enum State
            {
                Clear = 0,
                Turn = 1,
                Takes = 2,
                Stop = 4,
                Jack = 8,
                Ace = 16
            };

            void take( Card* t_card )
            {
                m_hand->push( t_card );
            }

            Card* throwCard( int t_cardIndex )
            {
                return m_hand->remove( t_cardIndex );
            }

            sf::TcpSocket* getSocket() const
            {
                return m_socket;
            }

            void setSocket( sf::TcpSocket* t_socket )
            {
                m_socket = t_socket;
            }

            sf::IpAddress getAddress() const
            {
                return m_address;
            }

            void setAddress( sf::IpAddress t_ip )
            {
                m_address = t_ip;
            }

            Deck* getHand() const
            {
                return m_hand;
            }

            State getState() const
            {
                return m_stateFlags;
            }

            void setState( State t_state )
            {
                m_stateFlags = t_state;
            }

            void addState( State t_state )
            {
                m_stateFlags = static_cast<State>( m_stateFlags | t_state );
            }

            int getId() const
            {
                return m_id;
            }

            friend sf::Packet& operator<<( sf::Packet& packet, const Player* player )
            {
                return packet << player->m_hand << player->m_stateFlags;
            }

            friend sf::Packet& operator>>( sf::Packet& packet, Player& player )
            {
                int tmp;
                packet >> player.m_hand >> tmp;
                player.m_stateFlags = static_cast<State>( tmp );
                return packet;
            }

            friend Game;
        private:
            Deck* m_hand = new Deck;

            int m_id;
            State m_stateFlags = Clear;
            sf::TcpSocket* m_socket;
            sf::IpAddress m_address;
    };
}
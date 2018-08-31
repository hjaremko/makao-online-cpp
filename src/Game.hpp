#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#include <Deck.hpp>
#include <Player.hpp>

namespace makao
{
    class Game
    {
        public:
            Game();
            Game( int );
            ~Game();

            void print() const;
            void print( const int ) const;
            void sendOut() const;
            void sendId( const int );
            void setTurn( int );
            void setPlayerSocket( int index, sf::TcpSocket* );
            void setPlayerAddress( int index, sf::IpAddress );
            void makeTurn( const int );
            void drawCard( int );
            int getChoice() const;
            int getPlayerAmount() const;
            Player::State getPlayerState( const int ) const;
            auto getTurnPlayer() const;
            bool isValid( int ) const;


            friend sf::Packet& operator<<( sf::Packet& packet, const Game& game )
            {
                packet << game.m_players.size();

                for ( auto it = game.m_players.begin(); it != game.m_players.end(); ++it )
                {
                    packet << ( *it ).get();
                }

                return packet << game.m_drawingDeck.get() << game.m_playingDeck.get();
            }

            friend sf::Packet& operator>>( sf::Packet& packet, Game& game )
            {
                int size;
                packet >> size;

                for ( int i = 0; i < size; ++i )
                {
                    auto tmp = std::make_shared<Player>();
                    packet >> *tmp;
                    game.m_players.push_back( tmp );
                }

                return packet >> game.m_drawingDeck.get() >> game.m_playingDeck.get();
            }

        private:
            void makePlayers( int );
            void makeDecks( int );

            std::vector<std::shared_ptr<Player>> m_players;
            std::shared_ptr<Deck> m_drawingDeck{ std::make_shared<Deck>() };
            std::shared_ptr<Deck> m_playingDeck{ std::make_shared<Deck>() };
    };
}

#pragma once

#include <Player.hpp>
#include <Deck.hpp>
#include <vector>
#include <iostream>
#include <sstream>

namespace makao
{
    class Game
    {
        friend sf::Packet& operator<<( sf::Packet&, const Game& );

        public:
            Game();
            Game( int );
            ~Game();

            void print() const;
            void print( int ) const;
            void sendOut() const;
            void sendId( const int );
            void setTurn( int );
            int getChoice() const;
            bool isValid( int );
            auto getTurnPlayer() const;

            Player::State getPlayerState( int t_id ) const
            {
                return m_players[ t_id ]->getState();
            }

            int getPlayerAmount() const
            {
                return m_players.size();
            }

            void setPlayerSocket( int index, sf::TcpSocket* t_socket )
            {
                m_players.at( index )->setSocket( t_socket );
            }

            void setPlayerAddress( int index, sf::IpAddress t_ip )
            {
                m_players.at( index )->setAddress( t_ip );
            }
            
            // std::string getPlayerId( int i ) const
            // {
                // std::istringstream s;
                // s << i << "_" << m_players[ i ]->getAddress().toString()
                //        << "_" << m_players[ i ]->getPort();
                // return s.str();
            // }

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

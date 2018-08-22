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
            Player* getTurnPlayer() const;

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

                for ( std::vector<Player*>::const_iterator it = game.m_players.begin(); it != game.m_players.end(); ++it )
                {
                    packet << *it;
                }

                return packet << game.m_drawingDeck << game.m_playingDeck;
            }

            friend sf::Packet& operator>>( sf::Packet& packet, Game& game )
            {
                int size;
                packet >> size;

                for ( int i = 0; i < size; ++i )
                {
                    Player* tmp = new Player();
                    packet >> *tmp;
                    game.m_players.push_back( tmp );
                }

                return packet >> game.m_drawingDeck >> game.m_playingDeck;
            }

        private:
            void makePlayers( int );
            void makeDecks( int );

            std::vector<Player*> m_players;
            Deck* m_drawingDeck;
            Deck* m_playingDeck;
    };
}

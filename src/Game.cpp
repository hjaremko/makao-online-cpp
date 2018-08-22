#include <iostream>
#include "Game.hpp"
#include "Deck.hpp"

namespace makao
{
    Game::Game()
    {
        m_drawingDeck = new Deck();
        m_playingDeck = new Deck();
    }

    Game::Game( int const t_playerAmount )
    {
        makePlayers( t_playerAmount );
        makeDecks( 52 );
        m_drawingDeck->shuffle();

        for ( Player* player : m_players )
        {
            m_drawingDeck->dealOut( 5, player );
        }

        m_playingDeck->push( m_drawingDeck->pop() );
    }

    Game::~Game()
    {
        for ( std::vector<Player*>::iterator i = m_players.begin(); i != m_players.end(); ++i )
        {
            delete *i;
        }

        delete m_drawingDeck;
        delete m_playingDeck;
    }

    void Game::makePlayers( int t_playerAmount )
    {
        for ( int i = 0; i < t_playerAmount; ++i )
        {
            m_players.push_back( new Player( i ) );
        }
    }

    void Game::makeDecks( const int t_size )
    {
        m_drawingDeck = new Deck();
        m_drawingDeck->make( t_size );

        m_playingDeck = new Deck();
    }

    void Game::sendId( const int t_id )
    {
        sf::Packet idPacket;
        idPacket << t_id;
        m_players.at( t_id )->m_socket->send( idPacket );
    }

    void Game::sendOut() const
    {
        std::cout << "Sending card info to players..." << std::endl;

        sf::Packet gamePacket;
        gamePacket << *this;

        for ( unsigned int i = 0; i < m_players.size(); ++i )
        {
            std::cout << "Sending to player " << i << std::endl;
            m_players[ i ]->m_socket->send( gamePacket );
        }
    }

    void Game::print() const
    {
        for ( int i = 0; i < m_players.size(); ++i )
        {
            std::cout << "Player " << i << " has: ";
            m_players[ i ]->getHand()->print();
            std::cout << std::endl;
        }

        std::cout << "Top card: " << m_playingDeck->peek()->str() << std::endl;
    }

    void Game::print( int t_id ) const
    {
        for ( int i = 0; i < m_players.size(); ++i )
        {
            std::cout << "Player " << i << " has: ";

            if ( i == t_id )
            {
                m_players[ i ]->getHand()->print();
            }
            else
            {
                std::cout << m_players[ i ]->getHand()->getSize() << " cards";
            }

            std::cout << std::endl;
        }

        std::cout << "Top card: " << m_playingDeck->peek()->str() << std::endl;
    }

    Player* Game::getTurnPlayer() const
    {
        for ( Player* player : m_players )
        {
            if ( player->getState() & Player::Turn )
            {
                return player;
            }
        }

        return nullptr;
    }

    void Game::setTurn( int t_id )
    {
        for ( Player* player : m_players )
        {
            player->setState( static_cast<Player::State>( player->getState() & ~Player::Turn ) );
        }

        m_players.at( t_id )->setState( Player::Turn );
    }

    int Game::getChoice() const
    {
        sf::Packet choicePacket;
        int choice;

        getTurnPlayer()->getSocket()->receive( choicePacket );
        
        if ( getTurnPlayer()->getState() & Player::Turn )
        {
            choicePacket >> choice;
            std::cout << "Got: " << choice << std::endl;

            return choice;
        }
        else
        {
            std::cout << "Not your turn!" << std::endl;
            return -1;
        }
    }

    bool Game::isValid( int t_choice )
    {
        try
        {
            if ( *( getTurnPlayer()->getHand()->at( t_choice )) == *(m_playingDeck->peek() ) )
            {
                m_playingDeck->push( getTurnPlayer()->throwCard( t_choice ) );
                return true;
            }
            else
            {
                return false;
            }
        }
        catch (...)
        {
            return false;
        }
    }
}

#include <iostream>
#include <exception>

#include "makaoexcept.hpp"
#include "Game.hpp"
#include "Deck.hpp"

namespace makao
{
    Game::Game()
    {
        Game( 2 );
    }

    Game::Game( const int t_playerAmount )
    {
        makePlayers( t_playerAmount );
        makeDecks();
        m_drawingDeck->shuffle();

        for ( auto player : m_players )
        {
            m_drawingDeck->dealOut( 5, player );
        }

        m_playingDeck->push( m_drawingDeck->pop() );
    }

    Game::~Game()
    {
    }

    void Game::makePlayers( const int t_playerAmount )
    {
        for ( int i = 0; i < t_playerAmount; ++i )
        {
            m_players.push_back( std::make_shared<Player>( i ) );
        }
    }

    void Game::makeDecks()
    {
        m_drawingDeck = std::make_shared<Deck>();
        m_playingDeck = std::make_shared<Deck>();
        m_drawingDeck->make();
    }

    void Game::sendId( const int t_id )
    {
        sf::Packet idPacket;
        idPacket << t_id;
        if ( m_players.at( t_id )->m_socket->send( idPacket ) == sf::Socket::Disconnected )
        {
            std::cout << "Failed sending id to player " << t_id << "!" << std::endl;
            throw LostConnectionException( t_id );
        }
    }

    void Game::sendOut() const
    {
        // std::cout << "Sending card info to players..." << std::endl;

        sf::Packet gamePacket;
        gamePacket << *this;

        for ( unsigned int i = 0; i < m_players.size(); ++i )
        {
            // std::cout << "Sending to player " << i << std::endl;

            if ( m_players[ i ]->m_socket->send( gamePacket ) == sf::Socket::Disconnected )
            {
                std::cout << "Failed sending game packet to player " << i << "!" << std::endl;
                throw LostConnectionException( i );
            }
        }
    }

    void Game::print() const
    {
        for ( std::size_t i = 0; i < m_players.size(); ++i )
        {
            std::cout << "Player " << i << " has: ";
            m_players[ i ]->getHand()->print();
            std::cout << std::endl;
        }

        std::cout << "Top card: " << m_playingDeck->peek()->str() << std::endl;
    }

    void Game::print( const unsigned int t_id ) const
    {
        for ( std::size_t i = 0; i < m_players.size(); ++i )
        {
            std::cout << "Player " << i << " has: ";

            if ( i == t_id )
            {
                m_players[ i ]->getHand()->print();
            }
            else
            {
                std::cout << m_players[ i ]->getHand()->getSize() << " cards ";
            }

            m_players[ i ]->printStates();
            std::cout << std::endl;
        }

        std::cout << "Top card: " << m_playingDeck->peek()->str() << std::endl;
        std::cout << "Cards in drawing deck: " << m_drawingDeck->getSize() << std::endl;
        std::cout << "Cards in playing stack: " << m_playingDeck->getSize() << std::endl;
    }

    auto Game::getTurnPlayer() const //!!!
    {
        for ( auto player : m_players )
        {
            if ( player->getState() & Player::Turn )
            {
                return player;
            }
        }

        return m_players[ 0 ];
    }

    void Game::setTurn( const int t_id )
    {
        for ( auto player : m_players )
        {
            player->setState( static_cast<Player::State>( player->getState() & ~Player::Turn ) );
        }

        m_players.at( t_id )->setState( Player::Turn );
    }

    int Game::getChoice() const
    {
        sf::Packet choicePacket;

        if ( getTurnPlayer()->getSocket()->receive( choicePacket ) == sf::Socket::Done )
        {
            if ( getTurnPlayer()->getState() & Player::Turn )
            {
                int choice = 0;
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
        else
        {
            return -1;
        }
    }

    bool Game::isValid( const int t_choice ) const
    {
        try
        {
            auto choosenCard = *( getTurnPlayer()->getHand()->at( t_choice ) );
            std::cout << "Choosen " << choosenCard.str() << std::endl;

            return ( choosenCard == *( m_playingDeck->peek() ) );
        }
        catch (...)
        {
            return false;
        }
    }

    void Game::makeTurn( const int t_choice )
    {
        m_playingDeck->push( getTurnPlayer()->throwCard( t_choice ) );
    }

    void Game::drawCard( int t_amount )
    {
        if ( m_drawingDeck->getSize() < t_amount )
        {
            std::cout << "Not enough cards to draw! Shuffling." << std::endl;
            shuffleDecks();
        }

        m_drawingDeck->dealOut( t_amount, getTurnPlayer() );
    }

    void Game::shuffleDecks()
    {
        for ( int i = 0; i != m_playingDeck->getSize() - 1; ++i )
        {
            m_drawingDeck->push( m_playingDeck->pop() );
        }

        m_drawingDeck->shuffle();
    }

    Player::State Game::getPlayerState( const int t_id ) const
    {
        return m_players[ t_id ]->getState();
    }

    int Game::getPlayerAmount() const
    {
        return m_players.size();
    }

    void Game::setPlayerSocket( const int index, sf::TcpSocket* t_socket )
    {
        m_players.at( index )->setSocket( t_socket );
    }

    void Game::setPlayerAddress( const int index, sf::IpAddress t_ip )
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
}

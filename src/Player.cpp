#include "Player.hpp"

namespace makao
{
    Player::Player() {}
    Player::Player( int t_id ) : m_id( t_id ) {}
    Player::~Player() {}

    void Player::take( std::shared_ptr<Card> t_card )
    {
        m_hand->push( t_card );
    }

    std::shared_ptr<Card> Player::throwCard( int t_cardIndex )
    {
        return m_hand->remove( t_cardIndex );
    }

    sf::TcpSocket* Player::getSocket() const
    {
        return m_socket;
    }

    void Player::setSocket( sf::TcpSocket* t_socket )
    {
        m_socket = t_socket;
    }

    sf::IpAddress Player::getAddress() const
    {
        return m_address;
    }

    void Player::setAddress( sf::IpAddress t_ip )
    {
        m_address = t_ip;
    }

    std::shared_ptr<Deck> Player::getHand() const
    {
        return m_hand;
    }

    Player::State Player::getState() const
    {
        return m_stateFlags;
    }

    void Player::setState( Player::State t_state )
    {
        m_stateFlags = t_state;
    }

    void Player::addState( Player::State t_state )
    {
        m_stateFlags = static_cast<State>( m_stateFlags | t_state );
    }

    int Player::getId() const
    {
        return m_id;
    }

    void Player::printStates() const
    {
        if ( m_stateFlags & Turn )
        {
            std::cout << "Turn" << " ";
        }

        if ( m_stateFlags & Take )
        {
            std::cout << "Takes" << " ";
        }

        if ( m_stateFlags & Stop )
        {
            std::cout << "Stop" << " ";
        }

        if ( m_stateFlags & Ace )
        {
            std::cout << "Ace" << " ";
        }

        if ( m_stateFlags & Jack )
        {
            std::cout << "Jack";
        }
    }
}
#include "Deck.hpp"
#include "Player.hpp"
#include <random>
#include <iostream>
#include <algorithm>
#include <ctime>

thread_local std::mt19937 gen{ std::random_device{}() };
 
template<typename T>
T random( T min, T max )
{
    return std::uniform_int_distribution<T>{ min, max }( gen );
}


namespace makao
{
    void Deck::make( int const t_size )
    {
        for ( int i = Card::Suit::hearts; i <= Card::Suit::spades; ++i )
        {
            for ( int j = Card::Figure::ace; j <= Card::Figure::king; ++j )
            {
                m_cards.push_back( std::make_shared<Card>( static_cast<Card::Suit>( i ), static_cast<Card::Figure>( j ) ) );
            }
        }
    }

    void Deck::shuffle()
    {
        std::mt19937 rng;
        rng.seed( time( 0 ) );

        std::shuffle( m_cards.begin(), m_cards.end(), rng );
    }

    void Deck::dealOut( int const amount, std::shared_ptr<Player> t_player )
    {
        for ( int i = 0; i < amount; ++i )
        {
            t_player->take( pop() );
        }
    }

    std::shared_ptr<Card> Deck::pop()
    {
        auto last = m_cards.back();
        m_cards.pop_back();
        return last;
    }

    std::shared_ptr<Card> Deck::remove( int t_cardIndex )
    {
        auto tmp = m_cards.at( t_cardIndex );
        m_cards.erase( m_cards.begin() +  t_cardIndex );

        return tmp;
    }

    std::shared_ptr<Card> Deck::peek() const
    {
        return m_cards.back();
    }

    void Deck::push( std::shared_ptr<Card> t_card )
    {
        m_cards.push_back( t_card );
    }

    void Deck::print() const
    {
        for ( int i = 0; i < m_cards.size(); ++i )
        {
            std::cout << m_cards[ i ]->str() << " ";
        }
    }
}

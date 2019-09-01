#include "makao-online/deck.hpp"

#include "makao-online/empty_deck.hpp"
#include "makao-online/server/player.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

thread_local std::mt19937 gen{ std::random_device{}() };

template <typename T>
T random( T min, T max )
{
    return std::uniform_int_distribution<T>{ min, max }( gen );
}

namespace makao
{
void deck::make()
{
    for ( int i = card::suit::hearts; i <= card::suit::spades; ++i )
    {
        for ( int j = card::figure::ace; j <= card::figure::king; ++j )
        {
            cards_.push_back( std::make_shared<card>( static_cast<card::suit>( i ),
                                                      static_cast<card::figure>( j ) ) );
        }
    }
}

void deck::shuffle()
{
    std::mt19937 rng;
    rng.seed( static_cast<unsigned int>( time( nullptr ) ) );

    std::shuffle( cards_.begin(), cards_.end(), rng );
}

void deck::deal_out( int amount, const std::shared_ptr<player>& player )
{
    for ( int i = 0; i < amount; ++i )
    {
        player->take_card( pop() );
    }
}

std::shared_ptr<card> deck::pop()
{
    if ( !cards_.empty() )
    {
        auto last = cards_.back();
        cards_.pop_back();
        return last;
    }
    else
    {
        throw empty_deck();
    }
}

std::shared_ptr<card> deck::remove( unsigned int card_index )
{
    auto tmp = cards_.at( card_index );
    cards_.erase( cards_.begin() + static_cast<int>( card_index ) );

    return tmp;
}

std::shared_ptr<card> deck::peek() const
{
    return cards_.back();
}

std::shared_ptr<card> deck::at( unsigned int i ) const
{
    return cards_.at( i );
}

unsigned deck::size() const
{
    return cards_.size();
}

void deck::push( const std::shared_ptr<card>& card )
{
    cards_.push_back( card );
}

void deck::print() const
{
    for ( auto& card : cards_ )
    {
        std::cout << card->to_string() << " ";
    }
}
} // namespace makao

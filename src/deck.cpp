#include "deck.hpp"

#include "empty_deck_exception.hpp"

#include <algorithm>
#include <ctime>
#include <random>

thread_local std::mt19937 gen { std::random_device {}() };

template <typename T>
T random( T min, T max )
{
    return std::uniform_int_distribution<T> { min, max }( gen );
}

namespace makao
{
deck make_full_deck()
{
    auto tmp { deck {} };

    for ( const auto suit : card::all_suits )
    {
        for ( const auto figure : card::all_figures )
        {
            tmp.cards_.emplace_back( suit, figure );
        }
    }

    tmp.shuffle();
    return tmp;
}

void deck::shuffle()
{
    std::mt19937 rng;
    rng.seed( static_cast<unsigned int>( time( nullptr ) ) );

    std::shuffle( cards_.begin(), cards_.end(), rng );
}

card deck::draw()
{
    if ( !cards_.empty() )
    {
        const auto last = cards_.back();
        cards_.pop_back();
        return last;
    }
    else
    {
        throw empty_deck_exception();
    }
}

void deck::push( const card& c )
{
    cards_.push_back( c );
}
} // namespace makao

#ifndef MAKAO_ONLINE_PLAYER_HAND_HPP
#define MAKAO_ONLINE_PLAYER_HAND_HPP

#include "card.hpp"

#include <list>
#include <optional>

namespace makao
{
class player_hand : public deck
{
public:
    void print() const
    {
        std::cout << "Cards: ";

        for ( const auto& card : cards_ )
        {
            std::cout << card.to_string() << ' ';
        }

        if ( frozen_for_ != 0 )
            std::cout << "\nFrozen for: " << frozen_for_ << '\n';
    }

    void print_concealed() const
    {
        std::cout << "Remaining cards: " << cards_.size();
        if ( cards_.size() == 1 )
            std::cout << " MAKAO";
        std::cout << '\n';

        if ( frozen_for_ != 0 )
            std::cout << "\nFrozen for: " << frozen_for_ << '\n';
    }

    [[nodiscard]] std::string serialize() const override
    {
        return std::to_string( frozen_for_ ) + ',' + deck::serialize();
    }

    void deserialize( const std::string& str ) override
    {
        auto tmp { str };
        auto h { player_hand {} };
        std::replace( std::begin( tmp ), std::end( tmp ), ',', ' ' );

        auto ss { std::stringstream { tmp } };
        auto frozen_for { 0 };
        auto hand_size { 0 };

        ss >> frozen_for >> hand_size;
        auto c { card { card::suit::diamonds, card::figure::ace } };

        for ( auto i { 0 }; i < hand_size; ++i )
        {
            std::string t;
            ss >> t;
            c.deserialize( t );
            h.push( c );
        }

        std::swap( *this, h );
    }

    void push( const card& card ) override
    {
        deck::push( card );
        sort();
    }

private:
    int frozen_for_ { 0 };
};
} // namespace makao
#endif

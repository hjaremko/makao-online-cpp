#ifndef MAKAO_ONLINE_DECK_HPP
#define MAKAO_ONLINE_DECK_HPP

#include "card.hpp"
#include "interfaces/serializable.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace makao
{

class deck : public serializable
{
public:
    friend deck make_full_deck();

    virtual void shuffle();
    virtual void push( const card& );
    virtual card draw();

    [[nodiscard]] size_t size() const
    {
        return cards_.size();
    };

    [[nodiscard]] const std::vector<card>& get_cards() const
    {
        return cards_;
    }

    [[nodiscard]] std::vector<card>& get_cards()
    {
        return cards_;
    }

    [[nodiscard]] const card& top() const
    {
        return cards_.back();
    }

    [[nodiscard]] std::string serialize() const override
    {
        std::stringstream ss;
        ss << cards_.size() << ',';

        for ( const auto& c : cards_ )
        {
            ss << c.serialize() << ',';
        }

        return ss.str();
    }

    void deserialize( const std::string& str ) override
    {
        auto size { 0 };
        auto tmp { str };
        auto d { deck {} };

        std::replace( std::begin( tmp ), std::end( tmp ), ',', ' ' );
        std::stringstream ss( tmp );

        ss >> size;
        auto c { card { card::suit::diamonds, card::figure::ace } };
        // std::cout << "Deserialization of " << size << " cards..." << std::endl;

        for ( int i = 0; i < size; ++i )
        {
            std::string t;
            ss >> t;
            c.deserialize( t );
            d.push( c );
        }

        std::swap( *this, d );
    }

    friend bool operator==( const deck& lhs, const deck& rhs )
    {
        return lhs.cards_ == rhs.cards_;
    }

    friend bool operator!=( const deck& lhs, const deck& rhs )
    {
        return !( rhs == lhs );
    }

    void sort()
    {
        std::sort( std::begin( cards_ ), std::end( cards_ ) );
    }

    //TODO: reshuffling

protected:
    std::vector<card> cards_;
};

deck make_full_deck();
} // namespace makao
#endif
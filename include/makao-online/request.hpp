#ifndef MAKAO_ONLINE_REQUEST_HPP
#define MAKAO_ONLINE_REQUEST_HPP

#include "card.hpp"

#include <iostream>
#include <unordered_map>
#include <variant>

namespace makao
{
const std::unordered_map<card::figure, std::string> figure_to_str;
const std::unordered_map<card::suit, std::string> suit_to_str;

class request : public serializable
{
public:
    request( const std::variant<card::figure, card::suit>& request, int lasting_for )
        : request_( request ), lasting_for_( lasting_for )
    {
    }

    friend bool operator==( const request& lhs, const request& rhs )
    {
        return std::tie( lhs.request_, lhs.lasting_for_ ) ==
               std::tie( rhs.request_, rhs.lasting_for_ );
    }

    friend bool operator!=( const request& lhs, const request& rhs )
    {
        return !( rhs == lhs );
    }

    void print() const
    {
        std::cout << "Request lasting for: " << lasting_for_ << '\n';

        if ( std::holds_alternative<card::figure>( request_ ) )
        {
            std::cout << makao::figure_to_str.at( std::get<card::figure>( request_ ) ) << '\n';
        }
        else
        {
            std::cout << makao::suit_to_str.at( std::get<card::suit>( request_ ) ) << '\n';
        }
    }

    [[nodiscard]] std::string serialize() const override
    {
        std::stringstream ss;

        if ( std::holds_alternative<card::figure>( request_ ) )
        {
            ss << 0 << ',' << static_cast<card::data_type>( std::get<card::figure>( request_ ) );
        }
        else
        {
            ss << 1 << ',' << static_cast<card::data_type>( std::get<card::suit>( request_ ) );
        }

        ss << ',' << lasting_for_;

        return ss.str();
    }
    void deserialize( const std::string& str ) override
    {
        auto figure_or_suit { 0 };
        auto value { 0 };
        auto lasting_for { 0 };

        sscanf_s( str.c_str(), "%d,%d,%d", &figure_or_suit, &value, &lasting_for );

        request_ =
            figure_or_suit == 0
                ? std::variant<card::figure, card::suit> { static_cast<card::figure>( value ) }
                : static_cast<card::suit>( value );

        lasting_for_ = lasting_for;
    }

private:
    std::variant<card::figure, card::suit> request_;
    int lasting_for_ { 0 };
};
} // namespace makao
#endif

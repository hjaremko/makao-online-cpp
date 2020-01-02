#ifndef MAKAO_ONLINE_CARD_HPP
#define MAKAO_ONLINE_CARD_HPP

#include "interfaces/serializable.hpp"

#include <SFML/Network.hpp>
#include <sstream>

namespace makao
{
class card : public serializable
{
public:
    using data_type = int;

    enum class suit : data_type
    {
        hearts,
        diamonds,
        clubs,
        spades
    };

    enum class figure : data_type
    {
        ace,
        two,
        three,
        four,
        five,
        six,
        seven,
        eight,
        nine,
        ten,
        jack,
        queen,
        king
    };

    constexpr static const suit all_suits[] {
        suit::hearts, suit::diamonds, suit::clubs, suit::spades
    };

    constexpr static const figure all_figures[] { figure::ace,   figure::two,   figure::three,
                                                  figure::four,  figure::five,  figure::six,
                                                  figure::seven, figure::eight, figure::nine,
                                                  figure::ten,   figure::jack,  figure::queen,
                                                  figure::king };

    card( suit, figure );
    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::string serialize() const override
    {
        std::stringstream ss;
        ss << static_cast<data_type>( suit_ ) << '_' << static_cast<data_type>( figure_ );
        return ss.str();
    }

    void deserialize( const std::string& str ) override
    {
        auto tmp_suit { 0 };
        auto tmp_fig { 0 };

        sscanf_s( str.c_str(), "%d_%d", &tmp_suit, &tmp_fig );

        *this = card( static_cast<card::suit>( tmp_suit ), static_cast<card::figure>( tmp_fig ) );
    }

    [[nodiscard]] bool is_compatible( const card& rhs ) const
    {
        return suit_ == rhs.suit_ || figure_ == rhs.figure_ || rhs.figure_ == card::figure::queen;
    }

    friend bool operator==( const card& lhs, const card& rhs )
    {
        return std::tie( lhs.suit_, lhs.figure_ ) == std::tie( rhs.suit_, rhs.figure_ );
    }

    friend bool operator!=( const card& lhs, const card& rhs )
    {
        return !( rhs == lhs );
    }

    friend bool operator<( const card& lhs, const card& rhs )
    {
        return std::tie( lhs.suit_, lhs.figure_ ) < std::tie( rhs.suit_, rhs.figure_ );
    }

    friend bool operator>( const card& lhs, const card& rhs )
    {
        return rhs < lhs;
    }

    friend bool operator<=( const card& lhs, const card& rhs )
    {
        return !( rhs < lhs );
    }

    friend bool operator>=( const card& lhs, const card& rhs )
    {
        return !( lhs < rhs );
    }

private:
    suit suit_ { suit::hearts };
    figure figure_ { figure::ace };
};
} // namespace makao
#endif

#ifndef MAKAO_ONLINE_GAME_STATE_HPP
#define MAKAO_ONLINE_GAME_STATE_HPP

#include "deck.hpp"
#include "interfaces/serializable.hpp"
#include "player_hand.hpp"
#include "request.hpp"
#include "temp_configs.hpp"

#include <array>
#include <ostream>
#include <sstream>

namespace makao
{
class game_state : public serializable
{
public:
    using players_hands = std::array<player_hand, MAX_PLAYERS>;

    [[nodiscard]] std::string serialize() const override
    {
        std::stringstream ss;
        ss << get_turning_player() << ' ' << get_current_deck().serialize() << ' '
           << current_stack_.serialize();

        for ( const auto& hand : get_current_hands() )
        {
            ss << ' ' << hand.serialize();
        }

        if ( get_request_for() )
        {
            ss << ' ' << 1 << ' ' << get_request_for()->serialize();
        }
        else
        {
            ss << ' ' << 0 << ' ' << "0,0,0";
        }

        return ss.str();
    }

    void deserialize( const std::string& str ) override
    {
        std::stringstream ss { str };
        auto state { game_state {} };
        auto tmp { std::string {} };

        ss >> state.turning_player_;
        ss >> tmp;
        state.current_deck_.deserialize( tmp );
        ss >> tmp;
        state.current_stack_.deserialize( tmp );

        for ( auto& hand : state.current_hands_ )
        {
            ss >> tmp;
            hand.deserialize( tmp );
        }

        auto is_request { 0 };
        ss >> is_request;

        if ( is_request == 1 )
        {
            ss >> tmp;
            auto req { request { card::figure::ace, 0 } };
            req.deserialize( tmp );
            state.request_for_ = req;
        }

        std::swap( *this, state );
    }

    friend game_state make_starting_state()
    {
        game_state state;
        state.current_deck_ = make_full_deck();

        for ( auto& hand : state.current_hands_ )
        {
            for ( int i = 0; i < 5; ++i )
            {
                hand.push( state.current_deck_.draw() );
            }

            hand.sort();
        }

        state.current_stack_.push( state.current_deck_.draw() );
        return state;
    }

    friend bool operator==( const game_state& lhs, const game_state& rhs )
    {
        return std::tie(
                   lhs.turning_player_, lhs.current_deck_, lhs.current_hands_, lhs.request_for_ ) ==
               std::tie(
                   rhs.turning_player_, rhs.current_deck_, rhs.current_hands_, rhs.request_for_ );
    }

    friend bool operator!=( const game_state& lhs, const game_state& rhs )
    {
        return !( rhs == lhs );
    }

    void print() const
    {
        std::cout << "\nTurning player: " << turning_player_ << '\n';
        std::cout << "Deck size:      " << current_deck_.size() << '\n';
        std::cout << "Stack size:     " << current_stack_.size() << '\n';
        if ( request_for_ )
            request_for_->print();
        else
            std::cout << "No request\n";

        std::cout << "Hands: \n";

        for ( const auto& hand : current_hands_ )
        {
            std::cout << '\t';
            hand.print();
            std::cout << '\n';
        }
    }

    [[nodiscard]] int get_turning_player() const
    {
        return turning_player_;
    }

    [[nodiscard]] const deck& get_current_deck() const
    {
        return current_deck_;
    }

    [[nodiscard]] const players_hands& get_current_hands() const
    {
        return current_hands_;
    }

    [[nodiscard]] players_hands& get_current_hands()
    {
        return current_hands_;
    }

    [[nodiscard]] const std::optional<request>& get_request_for() const
    {
        return request_for_;
    }

    void next_player()
    {
        turning_player_ = ( turning_player_ + 1 ) % MAX_PLAYERS;
    }

    // private:
    int turning_player_ { 0 };
    deck current_deck_;
    deck current_stack_;
    players_hands current_hands_;
    std::optional<request> request_for_;
};

game_state make_starting_state();
} // namespace makao
#endif

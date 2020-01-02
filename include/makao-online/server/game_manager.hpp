#ifndef MAKAO_ONLINE_GAME_MANAGER_HPP
#define MAKAO_ONLINE_GAME_MANAGER_HPP

#include "deck.hpp"
#include "game_state.hpp"
#include "interfaces/player_action_interface.hpp"

#include <queue>
#include <stack>

namespace makao
{

class game_manager
{
public:
    game_manager();

    void update()
    {
        if ( action_queue_.empty() )
        {
            return;
        }

        const auto result = ( *action_queue_.front() )( states_.back() );
        action_queue_.pop_front();
        states_.push_back( result );
    }

    [[nodiscard]] game_state get_current_state() const
    {
        return states_.back();
    }

    void restart()
    {
        if ( states_.size() > 1 )
        {
            action_queue_.clear();
            states_.clear();
            states_.push_back( make_starting_state() );
        }
    }

    void push_action( player_action_interface_ptr action )
    {
        action_queue_.push_back( std::move( action ) );
    }

private:
    // game_config config_;
    // logger logger_;
    std::deque<game_state> states_;
    std::deque<player_action_interface_ptr> action_queue_;
};
} // namespace makao

#endif

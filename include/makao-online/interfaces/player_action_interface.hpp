#ifndef MAKAO_ONLINE_PLAYER_ACTION_INTERFACE_HPP
#define MAKAO_ONLINE_PLAYER_ACTION_INTERFACE_HPP

namespace makao
{
class player_action_interface
{
public:
    virtual ~player_action_interface() = default;
    virtual game_state operator()( game_state gs ) = 0;
    //    virtual void ask_suit() = 0;
    //    virtual void ask_figure() = 0;
};

using player_action_interface_ptr = std::unique_ptr<player_action_interface>;

class draw : public player_action_interface
{
public:
    explicit draw( int p ) : drawing_player( p )
    {
    }

    game_state operator()( game_state gs ) override
    {
        std::cout << "Attempting to draw..." << std::endl;

        if ( drawing_player != gs.turning_player_ )
        {
            std::cout << "Player " << drawing_player << " is not turning now." << std::endl;
            return gs;
        }

        const auto card = gs.current_deck_.draw();
        gs.current_hands_.at( drawing_player ).push( card );
        gs.next_player();

        return gs;
    }

    int drawing_player { 0 };
};

class discard : public player_action_interface
{
public:
    explicit discard( int p, int c ) : discarding_player( p ), card_index( c )
    {
    }

    game_state operator()( game_state gs ) override
    {
        std::cout << "Attempting to discard..." << std::endl;

        if ( discarding_player != gs.turning_player_ )
        {
            std::cout << "Player " << discarding_player << " is not turning now." << std::endl;
            return gs;
        }

        // TODO: check if legal

        auto discarded =
            gs.get_current_hands().at( discarding_player ).get_cards().begin() + card_index;

        if ( !gs.current_stack_.top().is_compatible( *discarded ) )
        {
            std::cout << "Discarding " << discarded->to_string() << " on top of "
                      << gs.current_stack_.top().to_string() << " is not a legal move."
                      << std::endl;
            return gs;
        }

        gs.current_stack_.push( *discarded );
        gs.get_current_hands().at( discarding_player ).get_cards().erase( discarded );
        // TODO: evaluate hand and set next legal moves
        gs.next_player();

        return gs;
    }

    int discarding_player { 0 };
    int card_index { 0 };
};
} // namespace makao
#endif

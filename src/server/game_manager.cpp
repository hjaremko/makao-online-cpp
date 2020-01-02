#include "server/game_manager.hpp"

namespace makao
{
game_manager::game_manager()
{
    states_.push_back( make_starting_state() );
}
}

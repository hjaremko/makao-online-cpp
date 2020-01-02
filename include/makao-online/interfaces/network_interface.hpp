#ifndef MAKAO_ONLINE_NETWORK_INTERFACE_HPP
#define MAKAO_ONLINE_NETWORK_INTERFACE_HPP

#include "game_state.hpp"

namespace makao
{
class network_interface
{
public:
    virtual ~network_interface() = default;
    virtual void update() = 0;
    virtual void ping_gc() = 0;
    virtual int connected_players_count() = 0;
};
} // namespace makao
#endif

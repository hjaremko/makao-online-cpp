#include "server/old_player.hpp"

namespace makao
{
player::player() = default;

player::player( int t_id ) : id_( t_id )
{
}

void player::take_card( const std::shared_ptr<card>& card )
{
    hand_->push( card );
}

std::shared_ptr<card> player::throw_card( int card_index )
{
    return hand_->remove( card_index );
}

sf::TcpSocket* player::get_socket() const
{
    return socket_;
}

void player::set_socket( sf::TcpSocket* socket )
{
    socket_ = socket;
}

sf::IpAddress player::get_address() const
{
    return address_;
}

void player::set_address( sf::IpAddress ip )
{
    address_ = ip;
}

std::shared_ptr<deck> player::get_hand() const
{
    return hand_;
}

player::state player::get_state() const
{
    return state_flags_;
}

void player::set_state( state s )
{
    state_flags_ = s;
}

void player::add_state( state s )
{
    state_flags_ = static_cast<state>( state_flags_ | s );
}

int player::get_id() const
{
    return id_;
}

void player::print_states() const
{
    if ( state_flags_ & turn )
    {
        std::cout << "turn"
                  << " ";
    }

    if ( state_flags_ & take )
    {
        std::cout << "Takes"
                  << " ";
    }

    if ( state_flags_ & stop )
    {
        std::cout << "stop"
                  << " ";
    }

    if ( state_flags_ & ace )
    {
        std::cout << "ace"
                  << " ";
    }

    if ( state_flags_ & jack )
    {
        std::cout << "jack";
    }
}
} // namespace makao

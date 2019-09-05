#include "makao-online/server/game.hpp"

#include "makao-online/deck.hpp"
#include "makao-online/lost_connection.hpp"

#include <exception>
#include <iostream>

namespace makao
{
game::game() : game( 2 )
{
}

game::game( const int player_amount )
{
    make_players( player_amount );
    make_decks();
    drawing_deck_->shuffle();

    for ( const auto& player : players_ )
    {
        drawing_deck_->deal_out( 5, player );
    }

    playing_deck_->push( drawing_deck_->pop() );
}

void game::make_players( const int player_amount )
{
    for ( int i = 0; i < player_amount; ++i )
    {
        players_.push_back( std::make_shared<player>( i ) );
    }
}

void game::make_decks()
{
    drawing_deck_ = std::make_shared<deck>();
    playing_deck_ = std::make_shared<deck>();
    drawing_deck_->make();
}

void game::send_id( const unsigned int id )
{
    sf::Packet id_packet;
    id_packet << id;
    if ( players_.at( id )->socket_->send( id_packet ) == sf::Socket::Disconnected )
    {
        std::cout << "Failed sending id to player " << id << "!" << std::endl;
        throw lost_connection( static_cast<int>( id ) );
    }
}

void game::send_out() const
{
    // std::cout << "Sending card info to players..." << std::endl;

    sf::Packet gamePacket;
    gamePacket << *this;

    for ( unsigned int i = 0; i < players_.size(); ++i )
    {
        // std::cout << "Sending to player " << i << std::endl;

        if ( players_[ i ]->socket_->send( gamePacket ) == sf::Socket::Disconnected )
        {
            std::cout << "Failed sending game packet to player " << i << "!" << std::endl;
            throw lost_connection( i );
        }
    }
}

void game::print() const
{
    for ( std::size_t i = 0; i < players_.size(); ++i )
    {
        std::cout << "player " << i << " has: ";
        players_[ i ]->get_hand()->print();
        std::cout << std::endl;
    }

    std::cout << "Top card: " << playing_deck_->peek()->to_string() << std::endl;
}

void game::print( const unsigned int t_id ) const
{
    for ( std::size_t i = 0; i < players_.size(); ++i )
    {
        std::cout << "player " << i << " has: ";

        if ( i == t_id )
        {
            players_[ i ]->get_hand()->print();
        }
        else
        {
            std::cout << players_[ i ]->get_hand()->size() << " cards ";
        }

        players_[ i ]->print_states();
        std::cout << std::endl;
    }

    std::cout << "Top card: " << playing_deck_->peek()->to_string() << std::endl;
    std::cout << "Cards in drawing deck: " << drawing_deck_->size() << std::endl;
    std::cout << "Cards in playing stack: " << playing_deck_->size() << std::endl;
}

auto game::get_turn_player() const //!!!
{
    for ( auto player : players_ )
    {
        if ( player->get_state() & player::turn )
        {
            return player;
        }
    }

    return players_[ 0 ];
}

void game::set_turn( const unsigned int id )
{
    for ( const auto& player : players_ )
    {
        player->set_state( static_cast<player::state>( player->get_state() & ~player::turn ) );
    }

    players_.at( id )->set_state( player::turn );
}

int game::get_choice() const
{
    sf::Packet choice_packet;

    if ( get_turn_player()->get_socket()->receive( choice_packet ) == sf::Socket::Done )
    {
        if ( get_turn_player()->get_state() & player::turn )
        {
            int choice = 0;
            choice_packet >> choice;
            std::cout << "Got: " << choice << std::endl;

            return choice;
        }
        else
        {
            std::cout << "Not your turn!" << std::endl;

            return -1;
        }
    }
    else
    {
        return -1;
    }
}

bool game::is_valid( const unsigned int choice ) const
{
    try
    {
        auto chosen_card = *( get_turn_player()->get_hand()->at( choice ) );
        std::cout << "Choosen " << chosen_card.to_string() << std::endl;

        return ( chosen_card == *( playing_deck_->peek() ) );
    }
    catch ( ... )
    {
        return false;
    }
}

void game::make_turn( const int choice )
{
    playing_deck_->push( get_turn_player()->throw_card( choice ) );
}

void game::draw_card( int amount )
{
    if ( drawing_deck_->size() < amount )
    {
        std::cout << "Not enough cards to draw! Shuffling." << std::endl;
        shuffle_decks();
    }

    drawing_deck_->deal_out( amount, get_turn_player() );
}

void game::shuffle_decks()
{
    for ( auto i = 0u; i != playing_deck_->size() - 1; ++i )
    {
        drawing_deck_->push( playing_deck_->pop() );
    }

    drawing_deck_->shuffle();
}

player::state game::get_player_state( const int id ) const
{
    return players_[ id ]->get_state();
}

int game::get_player_amount() const
{
    return players_.size();
}

void game::get_player_socket( int index, sf::TcpSocket* socket )
{
    players_.at( index )->set_socket( socket );
}

void game::set_player_address( int index, sf::IpAddress ip )
{
    players_.at( index )->set_address( ip );
}

// std::string getPlayerId( int i ) const
// {
// std::istringstream s;
// s << i << "_" << players_[ i ]->get_address().toString()
//        << "_" << players_[ i ]->getPort();
// return s.to_string();
// }
} // namespace makao

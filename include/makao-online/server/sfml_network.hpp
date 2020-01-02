#ifndef MAKAO_ONLINE_SFML_NETWORK_HPP
#define MAKAO_ONLINE_SFML_NETWORK_HPP

#include "interfaces/network_interface.hpp"
#include "lost_connection_exception.hpp"
#include "server/game_manager.hpp"
#include "sfml_server_config.hpp"

#include <SFML/Network.hpp>
#include <iostream>
#include <set>
#include <utility>

namespace makao
{
inline sf::Packet& operator<<( sf::Packet& os, const serializable& object )
{
    return os << object.serialize();
}

inline void operator>>( sf::Packet& os, serializable& object )
{
    auto tmp { std::string {} };
    os >> tmp;
    object.deserialize( tmp );
}

class sfml_network : public network_interface
{
public:
    explicit sfml_network( game_manager& game ) : game_( game )
    {
        listen_status_ = listener_.listen( game_server_.server_config_.port );
        selector_.add( listener_ );
    }

    void update() override
    {
        std::cout << "Connected players: " << connected_players_count() << std::endl;

        if ( listening_ && listen_status_ == sf::Socket::Done )
        {
            listen_and_receive();
        }

        send_to_all( game_.get_current_state() );
    }

    void ping_gc() override
    {
        //        std::cout << "Sending server info to the coordinator..." << std::endl;
        coordinator_ << game_server_.server_config_;
        coordinator_.flush();
    }

    int connected_players_count() override
    {
        return std::count_if( std::begin( connected_players_ ),
                              std::end( connected_players_ ),
                              []( const auto& p ) { return p.has_value(); } );
    }

private:
    struct server
    {
        explicit server( server_config sc ) : server_config_( std::move( sc ) )
        {
            udp_socket_.setBlocking( false );
        }

        template <class T>
        friend sf::Packet& operator<<( server& s, const T& data )
        {
            return s.buffer_ << data;
        }

        void flush()
        {
            udp_socket_.send( buffer_, server_config_.ip, server_config_.port );
            buffer_.clear();
        }

        sf::UdpSocket udp_socket_;
        server_config server_config_;
        sf::Packet buffer_;
    };

    void add_player()
    {
        if ( selector_.isReady( listener_ ) && connected_players_count() < MAX_PLAYERS )
        {
            auto tmp { std::make_unique<sf::TcpSocket>() };
            tmp->setBlocking( false );

            if ( listener_.accept( *tmp ) == sf::Socket::Done )
            {
                std::cout << "New client connected: " << tmp->getRemoteAddress() << std::endl;

                selector_.add( *tmp );
                auto missing_player = std::distance( std::begin( connected_players_ ),
                                                     std::find( std::begin( connected_players_ ),
                                                                std::end( connected_players_ ),
                                                                std::nullopt ) );
                std::cout << "Missing: " << missing_player << std::endl;
                connected_players_.at( missing_player ) = std::move( tmp );
            }
        }
    }

    void send_to_all( const game_state& state )
    {
        for ( auto id = 0u; id < connected_players_.size(); ++id )
        {
            //            std::cout << "Sending game state to player " << ( *client
            //            )->getRemoteAddress() << "..."
            //                      << std::endl;
            if ( !connected_players_.at( id ) )
            {
                continue;
            }

            auto& client = *connected_players_.at( id );
            auto state_packet { sf::Packet {} };
            state_packet << id << state;

            if ( client->send( state_packet ) == sf::Socket::Disconnected )
            {
                std::cout << "Failed sending! Player " << id << " disconnected." << std::endl;
                std::cout << "Removing client " << client->getRemoteAddress() << std::endl;
                selector_.remove( *client );
                connected_players_.at( id ) = std::nullopt;
            }
        }
    }

    void receive_from_all()
    {
        std::cout << "Receiving..." << std::endl;

        for ( auto id = 0u; id < connected_players_.size(); ++id )
        {
            if ( !connected_players_.at( id ) )
            {
                continue;
            }

            auto& client = *connected_players_.at( id );

            if ( selector_.isReady( *client ) )
            {
                auto packet { sf::Packet {} };
                auto message { std::string {} };

                if ( client->receive( packet ) == sf::Socket::Done )
                {
                    packet >> message;
                }

                std::cout << "Got: " << message << " from " << id << std::endl;
                add_task( message, id );
            }
        }
    }

    void add_task( const std::string& message, int id )
    {
        if ( connected_players_count() != MAX_PLAYERS )
        {
            return;
        }

        auto ss { std::stringstream { message } };
        auto tmp { std::string {} };

        ss >> tmp;

        if ( tmp == "draw" )
        {
            game_.push_action( std::move( std::make_unique<draw>( id ) ) );
        }
        else if ( tmp == "discard" )
        {
            auto card_index { 0 };
            ss >> card_index;
            game_.push_action( std::move( std::make_unique<discard>( id, card_index ) ) );
        }
    }

    void listen_and_receive()
    {
        //        std::cout << "Listening for players..." << std::endl;

        if ( selector_.wait( sf::microseconds( 100 ) ) )
        {
            add_player();
            receive_from_all();
        }
    }

private:
    using connected_player = std::unique_ptr<sf::TcpSocket>;

    server game_server_ { { "Sample Server", "127.0.0.1", 55001, 0, MAX_PLAYERS } };
    server coordinator_ { { "__PING__", "127.0.0.1", 54000, 0, 0 } };

    bool listening_ { true };
    sf::Socket::Status listen_status_;
    sf::TcpListener listener_;
    sf::SocketSelector selector_;

    std::array<std::optional<connected_player>, MAX_PLAYERS> connected_players_;
    game_manager& game_;
    // logger& logger_;
};
} // namespace makao
#endif

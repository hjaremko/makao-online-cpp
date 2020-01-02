#include "game_state.hpp"

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

using namespace makao;

struct background_receiver
{
    explicit background_receiver( sf::TcpSocket& tcp_socket, bool& running )
        : tcp_socket( tcp_socket ), running( running )
    {
    }

    void operator()()
    {
        while ( running )
        {
            sf::Packet game_info_packet;
            auto status = tcp_socket.receive( game_info_packet );

            if ( status != sf::Socket::Done )
            {
                running = false;
                return;
            }
            //            else
            //            {
            //                std::cout << "received\n";
            //            }

            auto game_state_str = std::string();
            auto your_id = 0;

            game_info_packet >> your_id >> game_state_str;

            const auto last_state { current_state };
            current_state.deserialize( game_state_str );

            if ( game_state_str == last_state.serialize() )
            {
                continue;
            }

            std::cout << "You are player " << your_id << "\n";
            std::cout << "Turning player: " << current_state.turning_player_ << "\n\n";
            std::cout << "Deck size:      " << current_state.current_deck_.size() << '\n';
            std::cout << "Stack size:     " << current_state.current_stack_.size() << '\n';

            for ( auto i = 0u; i < current_state.current_hands_.size(); ++i )
            {
                std::cout << "Player " << i << ": \n\t";

                if ( i == your_id )
                {
                    current_state.current_hands_.at( your_id ).print();
                    std::cout << std::endl;
                }
                else
                {
                    current_state.current_hands_.at( i ).print_concealed();
                }
            }

            std::cout << "TOP CARD: " << current_state.current_stack_.top().to_string()
                      << std::endl;
        }
    }

    sf::TcpSocket& tcp_socket;
    game_state current_state;
    bool& running;
};

int main()
{
    bool running { true };
    sf::TcpSocket tcp_socket;

    auto status = tcp_socket.connect( "127.0.0.1", 55001 );

    if ( status != sf::Socket::Done )
        return 1;

    background_receiver receiver( tcp_socket, running );
    std::thread receiving_thread( receiver );

    std::cout << "[1] Draw\n";
    std::cout << "[2] Discard\n";

    auto choice = 0;
    while ( ( choice = std::cin.get() ) != 'q' )
    {
        auto action_packet { sf::Packet {} };
        switch ( choice )
        {
        case '1':
        {
            action_packet << "draw";
            status = tcp_socket.send( action_packet );
            break;
        }
        case '2':
        {
            std::cout << "Your discard > ";
            auto discard { 0 };
            std::cin >> discard;

            action_packet << ( "discard " + std::to_string( discard ) );
            status = tcp_socket.send( action_packet );
            break;
        }
        case '\n':
            break;
        default:
            std::cout << "Unknown action\n";
            break;
        }

        if ( status != sf::Socket::Done )
        {
            running = false;
            break;
        }
    }

    running = false;

    if ( receiving_thread.joinable() )
        receiving_thread.join();

    return 0;
}
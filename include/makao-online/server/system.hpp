#ifndef MAKAO_ONLINE_SYSTEM_HPP
#define MAKAO_ONLINE_SYSTEM_HPP

#include "game_manager.hpp"
#include "interfaces/system_interface.hpp"
#include "server/sfml_network.hpp"

#include <thread>

namespace makao
{
class system : public system_interface
{
public:
    system()
        : system_loop_( std::thread( &system::system_loop, this ) ),
          network_( std::make_unique<sfml_network>( game_manager_ ) )
    {
        // load config
    }

    ~system() override
    {
        is_running_ = false;

        if ( system_loop_.joinable() )
        {
            system_loop_.join();
        }
    }

    void system_loop() override
    {
        while ( is_running_ )
        {
            // std::cout << "Running..." << std::endl;
            game_manager_.get_current_state().print();
            network_->ping_gc();
            network_->update();

            if ( network_->connected_players_count() == MAX_PLAYERS )
            {
                game_manager_.update();
            }
            else
            {
                game_manager_.restart();
            }

            std::this_thread::sleep_for( refresh_interval );
        }
    }

    void stop() override
    {
        is_running_ = false;
    }

private:
    constexpr static const auto refresh_interval { std::chrono::milliseconds { 1000 } };

    game_manager game_manager_;
    std::unique_ptr<network_interface> network_ { nullptr };

    bool is_running_ { true };
    std::thread system_loop_;
};
} // namespace makao
#endif

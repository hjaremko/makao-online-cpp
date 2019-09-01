#pragma once

#include "makao-online/card.hpp"

#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace makao
{
class player;

class deck
{
public:
    void make();
    void shuffle();
    void deal_out( int, const std::shared_ptr<player>& );
    void push(const std::shared_ptr<card>& );
    void print() const;
    std::shared_ptr<card> pop();
    std::shared_ptr<card> remove( unsigned int );
    [[nodiscard]] std::shared_ptr<card> peek() const;
    [[nodiscard]] std::shared_ptr<card> at( unsigned int i ) const;
    [[nodiscard]] unsigned size() const;

    friend sf::Packet& operator<<( sf::Packet& packet, const deck& d )
    {
        packet << static_cast<int>(d.cards_.size());

        for ( const auto& m_card : d.cards_ )
        {
            packet << m_card.get();
        }

        return packet;
    }

    friend sf::Packet& operator>>( sf::Packet& packet, deck& d )
    {
        int size{ 0 };
        packet >> size;

        // std::cout << "Taking out " << size << " cards..." << std::endl;

        for ( int i = 0; i < size; ++i )
        {
            std::shared_ptr<card> tmp = std::make_shared<card>();
            packet >> *tmp;
            d.cards_.push_back( tmp );
        }

        return packet;
    }

private:
    std::vector<std::shared_ptr<card>> cards_;
};
} // namespace makao

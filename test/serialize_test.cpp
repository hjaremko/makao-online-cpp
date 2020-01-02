#include "catch\catch.hpp"
#include "game_state.hpp"

#include <iostream>

using namespace makao;

TEST_CASE( "Card is properly serialized and deserialized", "[serialize]" )
{
    for ( const auto s : card::all_suits )
    {
        for ( const auto f : card::all_figures )
        {
            auto card { makao::card( s, f ) };
            auto card2 { makao::card( card::suit::hearts, card::figure::ace ) };
            const auto serialized = card.serialize();

            card2.deserialize( serialized );
            REQUIRE( card2.serialize() == serialized );
            card.deserialize( serialized );
            REQUIRE( card.serialize() == serialized );
        }
    }
}

TEST_CASE( "Deck is properly serialized and deserialized", "[serialize]" )
{
    auto d { make_full_deck() };
    auto d2 { deck {} };
    d.shuffle();
    const auto org_cards { d.get_cards() };
    const auto serialized { d.serialize() };

    d2.deserialize( serialized );
    REQUIRE( d2.get_cards() == org_cards );
    REQUIRE( d2.serialize() == serialized );

    d.deserialize( serialized );
    REQUIRE( d.serialize() == serialized );
}

TEST_CASE( "Player Hand is properly serialized and deserialized", "[serialize]" )
{
    auto hand { player_hand {} };
    auto hand2 { player_hand {} };
    hand.push( card { card::suit::diamonds, card::figure::ace } );
    hand.push( card { card::suit::hearts, card::figure::ten } );
    hand.push( card { card::suit::spades, card::figure::four } );
    hand.push( card { card::suit::diamonds, card::figure::ace } );
    hand.push( card { card::suit::hearts, card::figure::queen } );

    const auto serialized { hand.serialize() };

    hand2.deserialize( serialized );
    REQUIRE( hand2.serialize() == serialized );
    hand.deserialize( serialized );
    REQUIRE( hand.serialize() == serialized );
}

TEST_CASE( "Empty Player Hand is properly serialized and deserialized", "[serialize]" )
{
    auto hand { player_hand {} };
    auto hand2 { player_hand {} };

    const auto serialized { hand.serialize() };

    hand2.deserialize( serialized );
    REQUIRE( hand2.serialize() == serialized );
    hand.deserialize( serialized );
    REQUIRE( hand.serialize() == serialized );
}

TEST_CASE( "Request is properly serialized and deserialized", "[serialize]" )
{
    for ( const auto& s : card::all_suits )
    {
        auto req { request { s, 123123 } };
        auto req2 { request { card::figure::four, 0 } };
        const auto serialized { req.serialize() };

        req2.deserialize( serialized );
        REQUIRE( req2.serialize() == serialized );
        req.deserialize( serialized );
        REQUIRE( req.serialize() == serialized );
    }

    for ( const auto& f : card::all_figures )
    {
        auto req { request { f, 12 } };
        auto req2 { request { card::suit::spades, 0 } };
        const auto serialized { req.serialize() };

        req2.deserialize( serialized );
        REQUIRE( req2.serialize() == serialized );
        req.deserialize( serialized );
        REQUIRE( req.serialize() == serialized );
    }
}

TEST_CASE( "Game State is properly serialized and deserialized", "[serialize]" )
{
    auto state { make_starting_state() };
    auto state2 { game_state {} };
    const auto serialized { state.serialize() };

//    std::cout << serialized << std::endl;

    state2.deserialize( serialized );
    REQUIRE( state2.serialize() == serialized );
    REQUIRE( state == state2 );
    state.deserialize( serialized );
    REQUIRE( state.serialize() == serialized );
}

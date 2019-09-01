project(makao_client)

set(SOURCE_FILES
        main.cpp
        client.cpp
        ../card.cpp
        ../deck.cpp
        ../server/game.cpp
        ../server/player.cpp
        ../server/server.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} project_warnings sfml-network)

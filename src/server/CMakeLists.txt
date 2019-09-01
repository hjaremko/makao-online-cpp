project(makao_server)

set(SOURCE_FILES
        main.cpp
        server.cpp
        game.cpp
        player.cpp
        ../card.cpp
        ../deck.cpp
        )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} project_warnings sfml-network Threads::Threads)

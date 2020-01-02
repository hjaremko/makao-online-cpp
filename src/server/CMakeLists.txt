project(makao_server)

set(SOURCE_FILES
        main.cpp
        ../card.cpp
        ../deck.cpp
        system.cpp
        game_manager.cpp
        sfml_network.cpp
        )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} project_warnings sfml-network Threads::Threads)

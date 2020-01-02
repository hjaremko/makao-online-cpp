project(makao_coordinator)

set(SOURCE_FILES main.cpp
        game_coordinator.cpp
        ../server/old_server.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} project_warnings sfml-network)

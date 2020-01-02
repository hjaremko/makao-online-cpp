cmake_minimum_required(VERSION 3.10)
project(makao_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        ../src/card.cpp
        ../src/deck.cpp
        )

set(TEST_FILES main_test.cpp
        serialize_test.cpp
        )

find_package(SFML 2.5 COMPONENTS network REQUIRED)
find_package(Threads REQUIRED)

include_directories("include")
include_directories("include/makao-online")

add_executable(makao_tests ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(${PROJECT_NAME} sfml-network Threads::Threads)
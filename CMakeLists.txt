cmake_minimum_required(VERSION 3.10)
project(makao_online
        VERSION 0.0.1
        DESCRIPTION "Multiplayer game of Mau Mau"
        LANGUAGES CXX)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include("cmake/clang-tools.cmake")
endif()

set(SFML_DIR "${SFML}/lib/cmake/SFML")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS network REQUIRED)

include_directories("include")
include_directories("include/makao-online")

add_subdirectory(src)
add_subdirectory(test)
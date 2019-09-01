cmake_minimum_required(VERSION 3.10)
project(makao_online
        VERSION 0.0.1
        DESCRIPTION "Multiplayer game of Mau Mau"
        LANGUAGES CXX)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS network REQUIRED)

include_directories("include")

add_subdirectory(src)
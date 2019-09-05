file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.hpp)

find_program(CLANG_TIDY NAMES clang-tidy)

if (CLANG_TIDY)
    add_custom_target(
            clang-tidy
            COMMAND ${CLANG_TIDY}
            ${SOURCE_FILES}
            ${HEADER_FILES}
            --
            -std=c++${CMAKE_CXX_STANDARD}
            -I ${CMAKE_SOURCE_DIR}/include
            -I${SFML_DIR}/../../../include
    )
else ()
    message(STATUS "clang-tidy not found")
endif ()

find_program(CLANG_FORMAT NAMES clang-format)

if (CLANG_FORMAT)
    add_custom_target(
            clang-format
            COMMAND ${CLANG_FORMAT}
            -i
            ${SOURCE_FILES}
            ${HEADER_FILES}
    )
else ()
    message(STATUS "clang-format not found")
endif ()
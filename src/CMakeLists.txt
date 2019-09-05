add_library(project_warnings INTERFACE)

if (MSVC)
    target_compile_options(project_warnings INTERFACE /W4)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(project_warnings
                         INTERFACE
                         -Wall
                         -Wextra
                         -Wshadow
                         -Wnon-virtual-dtor
                         -Wold-style-cast
                         -Wcast-align
                         -Wunused
                         -Woverloaded-virtual
                         -Wpedantic
                         -Wconversion
                         -Wsign-conversion
                         -Wmisleading-indentation
                         -Wduplicated-cond
                         -Wduplicated-branches
                         -Wlogical-op
                         -Wnull-dereference
                         -Wuseless-cast
                         -Wdouble-promotion
                         -Wformat=2
                         )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(project_warnings
                         INTERFACE
                         -Weverything
                         -Wno-padded
                         -Wno-c++98-compat
                         -Wno-c++98-compat-pedantic
                         )
endif ()

add_subdirectory(client)
add_subdirectory(coordinator)
add_subdirectory(server)

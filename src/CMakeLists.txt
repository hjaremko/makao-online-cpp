add_library(project_warnings INTERFACE)

if (MSVC)
    target_compile_options(project_warnings INTERFACE /W4)
else()
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
endif()

add_subdirectory(client)
add_subdirectory(coordinator)
add_subdirectory(server)

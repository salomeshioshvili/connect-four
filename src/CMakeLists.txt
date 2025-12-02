find_package(Threads REQUIRED)

add_library(connect4_library
    board.c
    game.c
    ai.c
    history.c
    io.c
)

target_link_libraries(connect4_library Threads::Threads)

add_executable(connect4 main.c)
target_link_libraries(connect4 connect4_library)

find_package(SDL2 QUIET)
if(SDL2_FOUND)
    add_library(connect4_graphics graphics.c)
    target_link_libraries(connect4_graphics SDL2::SDL2 m)
    target_link_libraries(connect4 connect4_graphics)
    target_compile_definitions(connect4 PRIVATE HAS_GRAPHICS=1)
endif()
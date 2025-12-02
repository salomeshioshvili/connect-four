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
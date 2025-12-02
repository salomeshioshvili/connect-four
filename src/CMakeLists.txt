find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)

add_library(connect4_library
    board.c
    game.c
    ai.c
    history.c
    io.c
    graphics.c
)

target_include_directories(connect4_library PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(connect4_library PRIVATE HAS_GRAPHICS)

target_link_libraries(connect4_library
    PRIVATE
        Threads::Threads
        SDL2::SDL2
        SDL2::SDL2main
        m
)

add_executable(connect4 main.c)
target_link_libraries(connect4 PRIVATE connect4_library)
target_compile_definitions(connect4 PRIVATE HAS_GRAPHICS)

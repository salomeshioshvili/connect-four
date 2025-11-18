add_library(connect4_library
 board.c
 game.c
 ai.c
 utils.c
 history.c
 io.c
)

target_include_directories(connect4_library PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_executable(connect4 main.c)
target_link_libraries(connect4 connect4_library)
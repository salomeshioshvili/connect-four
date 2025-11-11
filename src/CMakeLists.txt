find_package(SDL2 REQUIRED)

add_executable(main main.c)
target_link_libraries(main SDL2::SDL2)

add_library(library library.c)

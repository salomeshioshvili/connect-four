enable_testing()

add_executable(board_tests
    test_board.c
)

target_include_directories(board_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}   # for utest.h
)

target_link_libraries(board_tests PRIVATE connect4_library)

# Ensure the test binary is placed in a stable location and register test by target file
set_target_properties(board_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

add_test(NAME board_tests COMMAND $<TARGET_FILE:board_tests>)
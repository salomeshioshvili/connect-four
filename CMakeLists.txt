cmake_minimum_required(VERSION 3.21)
project(connect-four LANGUAGES C CXX)

include_directories(include)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
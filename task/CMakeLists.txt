cmake_minimum_required(VERSION 3.14)

project(tests LANGUAGES CXX)

add_subdirectory(proto)
add_subdirectory(parser)

enable_testing()
add_subdirectory(tests)
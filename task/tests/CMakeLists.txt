include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(
    parserTest parserTest.cpp
)

target_link_libraries(
    parserTest
    gtest
    gtest_main
    parser
)

include(GoogleTest)
gtest_discover_tests(parserTest)
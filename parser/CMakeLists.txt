include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(
    parser parserTools.h
    functions.h
)

target_link_libraries(parser PUBLIC proto)
cmake_minimum_required(VERSION 3.10)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Console version (original)
add_executable(tic-tac-toe-console main.cpp)

# Server version (REST API)
add_executable(tic-tac-toe-server server.cpp)
target_include_directories(tic-tac-toe-server PRIVATE ${CMAKE_SOURCE_DIR})

# Link pthread for httplib
target_link_libraries(tic-tac-toe-server PRIVATE pthread)

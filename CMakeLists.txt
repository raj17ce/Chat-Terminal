cmake_minimum_required(VERSION 3.16)
project(ChatTerminal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Client)
add_subdirectory(Server)

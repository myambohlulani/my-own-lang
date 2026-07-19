cmake_minimum_required(VERSION 4.2) #minimum version of cmake required

project(hlulani) #name of the project

set(CMAKE_CXX_STANDARD 20) # which version of cpp am i targeting
set(CMAKE_BUILD_TYPE Debug)

add_executable(radium src/main.cpp src/lexer.cpp src/parser.cpp)

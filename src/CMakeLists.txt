cmake_minimum_required(VERSION 3.19)
project(src C)

set(CMAKE_C_STANDARD 11)

add_executable(src main.c utils/fileParser.c utils/fileParser.h)
target_link_libraries(src m)
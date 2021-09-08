cmake_minimum_required(VERSION 3.19)
project(src C)

set(CMAKE_C_STANDARD 11)

# Uncomment when you run profiler
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(ea-equivalence main.c utils/fileParser.c utils/fileParser.h functions/partition.c functions/partition.h utils/structs.h functions/permutation.c functions/permutation.h utils/structs.c)
target_link_libraries(ea-equivalence m)
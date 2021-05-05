cmake_minimum_required(VERSION 3.19)
project(src C)

set(CMAKE_C_STANDARD 11)

add_executable(ea-equivalence main.c utils/fileParser.c utils/fileParser.h functions/partition.c functions/partition.h utils/structs.h functions/permutation.c functions/permutation.h utils/structs.c)
target_link_libraries(ea-equivalence m)
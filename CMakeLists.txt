cmake_minimum_required(VERSION 3.20)
project(ea_equivalence C)

set(CMAKE_C_STANDARD 11)

add_executable(ea_equivalence main.c utils/structs.h utils/parser.c utils/parser.h functions/partition.c functions/partition.h utils/freeMemory.c utils/freeMemory.h functions/outerPermutation.c functions/outerPermutation.h functions/innerPermutation.c functions/innerPermutation.h utils/basis.c utils/basis.h)

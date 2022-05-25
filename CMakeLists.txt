cmake_minimum_required(VERSION 3.20)
project(ea_equivalence C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(ea_equivalence
#        src/automorphism.c src/permutation.h src/permutation.c src/structures.h src/structures.c
#        src/order.c src/order.h src/structures.c src/structures.h
#        src/partition.c src/structures.h src/structures.c
        src/ea.c src/structures.c src/structures.h src/permutation.c src/permutation.h
        src/hybrid.c)

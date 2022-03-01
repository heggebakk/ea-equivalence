cmake_minimum_required(VERSION 3.20)
project(ea_equivalence C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(ea_equivalence
        main.c utils/parser.c utils/parser.h functions/partition.c functions/partition.h utils/freeMemory.c utils/freeMemory.h functions/outerPermutation.c functions/outerPermutation.h functions/innerPermutation.c functions/innerPermutation.h utils/linkedList.h utils/linkedList.c utils/inverse.c utils/inverse.h utils/compareTruthTable.c utils/compareTruthTable.h walshTransform/walshTransform.h walshTransform/walshTransform.c utils/eaTest.c utils/eaTest.h
        )

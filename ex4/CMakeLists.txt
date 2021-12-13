cmake_minimum_required(VERSION 3.19)
project(ex4 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex4 test_suite.c vector.c hashmap.c pair.c)
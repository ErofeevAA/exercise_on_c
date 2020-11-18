cmake_minimum_required(VERSION 3.17)
project(matrix C)

set(CMAKE_C_STANDARD 11)

add_executable(matrix main.c matrix.h matrix.c test.h test.c)
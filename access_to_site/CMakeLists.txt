cmake_minimum_required(VERSION 3.17)
project(access_to_site C)

set(CMAKE_C_STANDARD 11)

add_executable(access_to_site main.c user_list.h user_operation.c user_operation.h test.c test.h)
cmake_minimum_required(VERSION 3.17)
project(queue_in_shop C)

set(CMAKE_C_STANDARD 99)

add_executable(queue_in_shop main.c draw.c draw.h settings.txt shop_queue_model.c shop_queue_model.h shop_struct.h operation_shop_struct.c operation_shop_struct.h)

find_package( Curses REQUIRED )

include_directories( ${CURSES_INCLUDE_DIRS} )

target_link_libraries(queue_in_shop ${CURSES_LIBRARIES} )
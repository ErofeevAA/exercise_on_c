cmake_minimum_required(VERSION 3.17)
project(online_cinema C)

set(CMAKE_C_STANDARD 99)

add_executable(online_cinema main.c view.c view.h cinema.c cinema.h film_operation.c film_operation.h user.h film.h user_operation.c user_operation.h)

find_package( Curses REQUIRED )

include_directories( ${CURSES_INCLUDE_DIRS} )
target_link_libraries(online_cinema ${CURSES_LIBRARIES} )
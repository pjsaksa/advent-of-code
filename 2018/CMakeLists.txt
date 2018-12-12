cmake_minimum_required(VERSION 3.10)
project(aoc-2018)

#

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O2 -Wall -Wextra -Werror -fdiagnostics-color=always)

# 01

add_executable(01a 01a.cc)
add_executable(01b 01b.cc)
file(COPY input-01.txt DESTINATION .)

# 02

add_executable(02a 02a.cc)
add_executable(02b-1 02b-1.cc)
add_executable(02b-2 02b-2.cc)
file(COPY input-02.txt DESTINATION .)

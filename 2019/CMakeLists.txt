cmake_minimum_required(VERSION 3.0)
project(aoc-2019)

#

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3 -Wall -Wextra -Werror -fdiagnostics-color=always)

# 01

add_executable(01a 01a.cc)
add_executable(01b 01b.cc)
file(COPY input-01.txt DESTINATION .)

# 02

add_executable(02a 02a.cc intcode.cc)
add_executable(02b 02b.cc intcode.cc)
file(COPY input-02.txt DESTINATION .)

# 03

add_executable(03 03.cc)
file(COPY input-03.txt DESTINATION .)

# 04

add_executable(04-1 04-1.cc)
add_executable(04-2 04-2.cc)

# 05

add_executable(05a 05a.cc intcode.cc)
add_executable(05b 05b.cc intcode.cc)
file(COPY input-05.txt DESTINATION .)

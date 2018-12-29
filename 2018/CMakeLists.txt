cmake_minimum_required(VERSION 3.10)
project(aoc-2018)

#

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3 -Wall -Wextra -Werror -fdiagnostics-color=always)

# 01

add_executable(01a 01a.cc)
add_executable(01b 01b.cc)
file(COPY input-01.txt DESTINATION .)

# 02

add_executable(02a 02a.cc)
add_executable(02b-1 02b-1.cc)
add_executable(02b-2 02b-2.cc)
file(COPY input-02.txt DESTINATION .)

# 03

add_executable(03a-1 03a-1.cc)
add_executable(03a-2 03a-2.cc)
target_link_libraries(03a-2 pthread)
add_executable(03b 03b.cc)
file(COPY input-03.txt DESTINATION .)

# 04

add_executable(04a 04a.cc 04-common.cc)
add_executable(04b 04b.cc 04-common.cc)
file(COPY input-04.txt DESTINATION .)

# 05

add_executable(05a 05a.cc)
add_executable(05b 05b.cc)
file(COPY input-05.txt DESTINATION .)

# 06

add_executable(06a 06a.cc)
add_executable(06b 06b.cc)
file(COPY input-06.txt DESTINATION .)

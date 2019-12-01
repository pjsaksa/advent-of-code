cmake_minimum_required(VERSION 3.0)
project(aoc-2019)

#

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3 -Wall -Wextra -Werror -fdiagnostics-color=always)

# 01

add_executable(01a 01a.cc)
add_executable(01b 01b.cc)
file(COPY input-01.txt DESTINATION .)

cmake_minimum_required(VERSION 3.15)
project(aoc-2022 LANGUAGES NONE)

enable_testing()

add_subdirectory(day01)
add_subdirectory(util)

add_subdirectory(server)

# install

install(
	DIRECTORY "data"
	DESTINATION / )


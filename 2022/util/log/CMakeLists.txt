# test

add_test(
	NAME util/log
	COMMAND go test -v
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

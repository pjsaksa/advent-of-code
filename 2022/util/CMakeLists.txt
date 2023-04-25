set(CWD "${CMAKE_CURRENT_SOURCE_DIR}")

set(SERVER_DEP "${SERVER_DEP}"
	#
	"${CWD}/go.mod"
	#
	"${CWD}/error.go"
	"${CWD}/types.go"
	#
	PARENT_SCOPE )

# test

add_test(
	NAME util
	COMMAND go test
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

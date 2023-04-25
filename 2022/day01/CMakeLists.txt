set(CWD "${CMAKE_CURRENT_SOURCE_DIR}")

set(SERVER_DEP "${SERVER_DEP}"
	#
	"${CWD}/go.mod"
	#
	"${CWD}/main.go"
	#
	PARENT_SCOPE )

# test

add_test(
	NAME day-01
	COMMAND go test
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

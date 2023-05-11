set(CWD "${CMAKE_CURRENT_SOURCE_DIR}")

set(SERVER_DEP "${SERVER_DEP}"
	#
	"${CWD}/go.mod"
	#
	"${CWD}/main.go" )

# build

set(SERVER_BIN "${CMAKE_CURRENT_BINARY_DIR}/server")

add_custom_command(
	OUTPUT "${SERVER_BIN}"
	COMMAND go build -o "${SERVER_BIN}"
	COMMAND strip "${SERVER_BIN}"
	DEPENDS "${SERVER_DEP}"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

add_custom_target(
	server
	ALL
	DEPENDS "${SERVER_BIN}" )

# install

install(
	PROGRAMS "${SERVER_BIN}"
	DESTINATION / )

install(
	FILES "close-outline.svg"
	DESTINATION /static/img )

# test

add_test(
	NAME server
	COMMAND go test
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

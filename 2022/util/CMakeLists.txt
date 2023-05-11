set(CWD "${CMAKE_CURRENT_SOURCE_DIR}")

set(SERVER_DEP "${SERVER_DEP}"
	#
	"${CWD}/go.mod"
	#
	"${CWD}/http.go"
	"${CWD}/index.go"
	"${CWD}/types.go"
	#
	"${CWD}/log/main.go"
	#
	PARENT_SCOPE )

# install

install(
	FILES "index.css" "index.html"
	DESTINATION /static )

# test

add_test(
	NAME util
	COMMAND go test
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

#

add_subdirectory(log)

set(CWD "${CMAKE_CURRENT_SOURCE_DIR}")

set(SERVER_DEP "${SERVER_DEP}"
	#
	"${CWD}/go.mod"
	#
	"${CWD}/css.go"
	"${CWD}/graph.go"
	"${CWD}/graph-js.go"
	"${CWD}/info.go"
	"${CWD}/init.go"
	"${CWD}/text.go"
	#
	"${CWD}/main.go"
	#
	PARENT_SCOPE )

# install

install(
	FILES "01.css" "graph.js"
	DESTINATION /static/01 )

# test

add_test(
	NAME day-01
	COMMAND go test
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

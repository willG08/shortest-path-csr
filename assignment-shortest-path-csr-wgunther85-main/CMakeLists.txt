# Set minimum CMAKE
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project( ShortestPath )

# require C++17
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( APP_EXECUTABLE ShortestPathTest )

include_directories( inc )

ADD_EXECUTABLE( ${APP_EXECUTABLE} "src/shortest_path_test.cpp"  "src/shortest_path_csr.cpp")
#TARGET_LINK_LIBRARIES( ${APP_EXECUTABLE} ${LIB_NAME} )

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT ${APP_EXECUTABLE} )

add_custom_command(
        TARGET ${APP_EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/files/rome99.gr
                ${CMAKE_CURRENT_BINARY_DIR}/rome99.gr
)

include (CTest)
add_test(${APP_EXECUTABLE}_testBasicGraph ${APP_EXECUTABLE} 1)
add_test(${APP_EXECUTABLE}_testLargeGraph ${APP_EXECUTABLE} 2)


find_program(VALGRIND "valgrind")
if(VALGRIND)
	add_custom_target(valgrind
    COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --error-exitcode=1 $<TARGET_FILE:${APP_EXECUTABLE}>)
endif()
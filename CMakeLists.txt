#cmake file

cmake_minimum_required(VERSION 3.5)

project(mineswepper)

set(LIBRARY_MW "mw")
set(SRC_MAIN sources/main.cpp)
file(GLOB SRC_MW "sources/libminesweeper/*.cpp")

set (CMAKE_CXX_COMPILER clang++)
add_definitions(-Wall -std=c++14 -g)

include_directories(include)

find_package(Curses REQUIRED)
include_directories(${CURSES_INClUDE_DIR})

add_library(${LIBRARY_MW} SHARED ${SRC_MW})

add_executable(${PROJECT_NAME} ${SRC_MAIN})

target_link_libraries(${PROJECT_NAME} ${LIBRARY_MW} ${CURSES_LIBRARIES})

install(TARGETS ${LIBRARY_MW} DESTINATION /usr/lib)
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

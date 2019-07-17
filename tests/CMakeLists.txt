cmake_minimum_required(VERSION 3.0.0)

project(children_tests)

set(HEADERS test.h ../src/data.h)
set(SOURCES test.cpp main.cpp ../src/data.cpp)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT} ${HEADERS} ${SOURCES})

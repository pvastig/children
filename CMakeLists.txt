cmake_minimum_required(VERSION 3.8)

set PROJECT children
project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)

file(GLOB AUTO_SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT} ${AUTO_SOURCES})

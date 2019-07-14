cmake_minimum_required(VERSION 3.0.0)

project(array VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

file(GLOB AUTO_SOURCES "*.cpp" "*.h")

add_executable(children ${AUTO_SOURCES})

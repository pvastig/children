cmake_minimum_required(VERSION 3.8)

set(PROJECT children)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)

file(GLOB AUTO_SOURCES "src/*.cpp" "src/*.h")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT} ${AUTO_SOURCES})

target_link_libraries(${PROJECT} Threads::Threads)

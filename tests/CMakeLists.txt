cmake_minimum_required(VERSION 3.15)

set(PROJECT children_tests)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(HEADERS test.h ../src/data.h)
set(SOURCES test.cpp main.cpp ../src/data.cpp ../src/utils.cpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} Threads::Threads)

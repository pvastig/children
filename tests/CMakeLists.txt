cmake_minimum_required(VERSION 3.0.0)

set(PROJECT children_tests)
project(${PROJECT})

set(CMAKE_BUILD_TYPE RELEASE)
set(HEADERS test.h ../src/data.h)
set(SOURCES test.cpp main.cpp ../src/data.cpp ../src/utils.cpp)
set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} Threads::Threads)

cmake_minimum_required(VERSION 3.15)

set(PROJECT children)
project(${PROJECT})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single Directory for all"
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH
    "Single Directory for all"
    )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib
    CACHE PATH
    "Single Directory for all"
    )

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB AUTO_SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT} ${AUTO_SOURCES})
#TODO: add precompiled header
#target_precompile_headers(${PROJECT} PRIVATE src/user_types.h)
target_link_libraries(${PROJECT} Threads::Threads)

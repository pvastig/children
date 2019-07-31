cmake_minimum_required(VERSION 3.8)

set(PROJECT children)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB AUTO_SOURCES "src/*.cpp" "src/*.h")

add_executable(${PROJECT} ${AUTO_SOURCES})
target_link_libraries(${PROJECT} Threads::Threads)

if(MSVC)
  target_compile_options(${PROJECT} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

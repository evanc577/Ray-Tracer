cmake_minimum_required(VERSION 3.10.3)
project(mp1 CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)
include_directories(${PNG_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp")

add_executable(mp1 ${SOURCES})

find_package(Threads)
target_link_libraries (mp1 ${CMAKE_THREAD_LIBS_INIT})

find_package(PNG REQUIRED)
add_library(PNG SHARED IMPORTED)
target_link_libraries(mp1 ${PNG_LIBRARIES})

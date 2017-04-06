cmake_minimum_required(VERSION 3.5)
project(ImageDedup)

set(CMAKE_CXX_STANDARD 11)

# Boost
find_package(Boost COMPONENTS filesystem timer program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES} -lboost_filesystem -lboost_system)

set(SOURCE_FILES main.cpp)
add_executable(ImageDedup ${SOURCE_FILES})
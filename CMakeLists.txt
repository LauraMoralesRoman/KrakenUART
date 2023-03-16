cmake_minimum_required(VERSION 3.10)

project(ProtocolV3)

# Specify standatd
# set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")

# Libraries
add_subdirectory(shared)

add_compile_options(-Wall -fanalyzer)
add_executable(ProtocolV3 main.cpp)
target_link_libraries(ProtocolV3 protocol -lpthread)

# Custom targets
add_custom_target(run
    COMMAND ./ProtocolV3
    DEPENDS ./ProtocolV3)

add_custom_target(check
    COMMAND cppcheck --std=c++17 --enable=all --inconclusive --inline-suppr ../shared
    DEPENDS)


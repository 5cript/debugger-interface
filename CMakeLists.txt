# Version check
cmake_minimum_required(VERSION 3.9)

# Project
project(gdbint VERSION 1.0.0 DESCRIPTION "a very basic gdb mi parser")

# Add files
file(GLOB sources "*.cpp"
				  "input/*.cpp"
				  "input/parser/*.cpp"
				  "input/raw/*.cpp"
				  "../twisted-spirit/rules/space.cpp"
)

# Add library
add_library(gdbint SHARED ${sources})

set_target_properties(gdbint PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(gdbint PRIVATE "..")

find_library(LTINY_PROCESS_LIBRARY tiny-process-library PATHS "../tiny-process-library/build")

target_link_libraries(gdbint PRIVATE 
	"${LTINY_PROCESS_LIBRARY}"
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -s -static-libstdc++ -static-libgcc")

# Compiler Options
target_compile_options(gdbint PRIVATE -fexceptions -std=c++17 -Wall -pedantic-errors -pedantic)
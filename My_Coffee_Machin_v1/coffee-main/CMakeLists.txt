cmake_minimum_required(VERSION 3.20)
project(my_project)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)

# Add source files
add_executable(my_project src/main.cpp src/coffee_machine.cpp)
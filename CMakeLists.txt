cmake_minimum_required(VERSION 3.10)
project(MyToDo LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE "/home/bogdans/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

add_executable(my_todo src/main.cpp)


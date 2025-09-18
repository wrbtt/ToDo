cmake_minimum_required(VERSION 3.10)
project(MyToDo LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE "/home/bogdans/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)


find_path(IMGUI_INCLUDE_DIR imgui.h PATHS "/home/bogdans/vcpkg/installed/x64-linux/include")
find_library(IMGUI_LIBRARY imgui PATHS "/home/bogdans/vcpkg/installed/x64-linux/lib")


add_executable(todo_console
    src/ToDo.cpp
    src/main.cpp
)

add_executable(todo_gui
    src/imgui_impl.cpp  
    src/ToDo.cpp
)

target_include_directories(todo_console PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(todo_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_INCLUDE_DIR}
)

target_link_libraries(todo_gui PRIVATE
    ${IMGUI_LIBRARY}
    glfw
    OpenGL::GL
    dl
    pthread
)

set_target_properties(todo_console todo_gui PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
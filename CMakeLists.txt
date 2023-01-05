cmake_minimum_required(VERSION 3.14)
project(simpDoom)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(SRC_LIST main.cpp src/traverser.cpp head/traverser.h src/error.cpp head/error.h)
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets 3.2 REQUIRED)
include(${wxWidgets_USE_FILE})
add_executable(simpDoom ${SRC_LIST})
target_link_libraries(simpDoom ${wxWidgets_LIBRARIES})
set(wxWidgets_USE_LIBS ON)


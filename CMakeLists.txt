cmake_minimum_required(VERSION 3.3)
project(BlueHarvest CXX)
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/resources/media/icons)
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
#add_definitions(${GTK3_CFLAGS_OTHER})
set(CMAKE_CXX_FLAGS "-Wall -export-dynamic -rdynamic")
# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
# Add an executable compiled from main.cc
add_executable(gne ${CMAKE_SOURCE_DIR}/src/main.cc src/loadsave.h src/struct.h src/manipulation.h src/manipulation.cc src/loadsave.cc src/handlers.cc src/handlers.h src/gtkfun.cc src/gtkfun.h)
# Link the target to the GTK+ libraries
target_link_libraries(gne ${GTK3_LIBRARIES})
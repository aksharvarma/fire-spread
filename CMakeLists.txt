cmake_minimum_required (VERSION 2.8)
project(fire_spread)
find_package(OpenCV REQUIRED)

# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

# Declare the executable target built from your sources
add_executable(f simulation.c spreads.c utils.c)
target_link_libraries( f ${OpenCV_LIBS} )

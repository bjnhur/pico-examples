cmake_minimum_required(VERSION 2.8.12)

# Set the project root directory.
set(WIZNET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Application)
add_subdirectory(Ethernet)
add_subdirectory(Internet)

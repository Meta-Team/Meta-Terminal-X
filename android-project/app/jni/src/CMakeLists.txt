cmake_minimum_required(VERSION 3.6)

project(MY_APP)

find_library(SDL2 SDL2)

find_library(SDL2main SDL2main)

add_library(main SHARED)

target_sources(main PRIVATE main.cpp)

target_link_libraries(main SDL2main SDL2)

target_include_directories(main PRIVATE ${SDL-Base}/include)

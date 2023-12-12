cmake_minimum_required(VERSION 3.10.0)

project(Meta-Terminal-X LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Meta-Terminal-X-Windows-SRC-DIR src)
include_directories(include)

# enable Qt ui resource generation and header finding
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_BINARY_DIR})
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core Charts Gui REQUIRED)

add_executable(Meta-Terminal-X
    ${Meta-Terminal-X-Windows-SRC-DIR}/main.cpp
    ${Meta-Terminal-X-Windows-SRC-DIR}/main.ui
    ${Meta-Terminal-X-Windows-SRC-DIR}/ThemeWidget.cpp
    # hpp header need to be added for MOC
    include/ThemeWidget.hpp
    #resources.qrc
)

target_link_libraries(Meta-Terminal-X Qt5::Widgets Qt5::Core Qt5::Charts Qt5::Gui)
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
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core Charts Gui SerialPort REQUIRED)
# In order to place ui file different from include directory
# see https://stackoverflow.com/questions/40630820/how-to-place-header-and-ui-file-in-different-folders-using-autouic-in-cmake
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/src)
add_executable(Meta-Terminal-X
    # hpp header need to be added for AUTOUIC
    # hpp header need to be added for AUTOMOC
    # Meta Object Compiler( needed by Q_OBJECT stuffs, otherwise it will generate errors like 
    # unresolved external symbol:"public: virtual int __cdecl ConsoleBox::qt_metacall)
    src/main.cpp
    include/TerminalWindow.hpp
    include/ControlPanel.hpp
    include/ConsoleBox.hpp
    include/SerialConfigDialog.hpp
    include/DynamicChart.hpp
    src/TerminalWindow.cpp
    src/ControlPanel.cpp
    src/ConsoleBox.cpp
    src/SerialConfigDialog.cpp
    src/DynamicChart.cpp
    src/resources.qrc
)

target_link_libraries(Meta-Terminal-X Qt5::Widgets Qt5::Core Qt5::Charts Qt5::Gui Qt5::SerialPort)
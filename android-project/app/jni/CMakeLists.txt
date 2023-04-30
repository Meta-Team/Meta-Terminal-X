cmake_minimum_required(VERSION 3.6)

project(Meta-Terminal-X-Android)

# armeabi-v7a requires cpufeatures library
# include(AndroidNdkModules)
# android_ndk_import_module_cpufeatures()
CMAKE_PATH(SET SDL-Base NORMALIZE "${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty/SDL2")

# SDL sources are in a subfolder named "SDL"
add_subdirectory(${SDL-Base} buildSDL)

# Compilation of companion libraries
#add_subdirectory(SDL_image)
#add_subdirectory(SDL_mixer)
#add_subdirectory(SDL_ttf)

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_subdirectory(src)


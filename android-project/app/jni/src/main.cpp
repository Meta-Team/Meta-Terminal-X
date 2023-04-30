#include <SDL.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <ctime>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <glm/glm.hpp>

#define LOGI(TAG, ...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGD(TAG, ...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

int SDL_main(int argc, char **argv) {
    //The window we'll be rendering to
    SDL_Window *window = nullptr;

    //The surface contained by the window
    SDL_Surface *screenSurface = nullptr;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        LOGE("Fatal", "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_Log("SDL initialized successfully!\n");


    //Set GLES Context 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //用于存放屏幕长宽等数据
    int nNumDiaplays = SDL_GetNumVideoDisplays();
    SDL_Rect rectDisplay;
    SDL_GetDisplayBounds(0, &rectDisplay);

    LOGI("[Display]", "number of Displays%d,width:%d,height:%d", nNumDiaplays, rectDisplay.w,
         rectDisplay.h);
    //Create window
    window = SDL_CreateWindow("Meta-Terminal-X", 0, 0, rectDisplay.w, rectDisplay.h,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    //Get window surface
    screenSurface = SDL_GetWindowSurface(window);

    //Fill the surface white
    SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0x66, 0xCC, 0xFF));
    //Update the surface
    SDL_UpdateWindowSurface(window);
    //Hack to get window to stay up
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
    }

    LOGI("[End]", "Goodbye!");


    return 0;
}
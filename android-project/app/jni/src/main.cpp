#include <SDL.h>
#include <android/log.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
int SDL_main(int argc,char** argv){
    //The window we'll be rendering to
    SDL_Window* window = NULL;

    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        SDL_Log( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }else
    {
        SDL_Log( "SDL initialized successfully!\n");
        //Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);

            //Fill the surface white
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

            //Update the surface
            SDL_UpdateWindowSurface(window);

            //Hack to get window to stay up
            SDL_Event e;
            bool quit = false;
            while (quit == false) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT)
                        quit = true;
                }
            }
        }
    }

    return 0;
}
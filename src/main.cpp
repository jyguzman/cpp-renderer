#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "renderer.hpp"

int main(int argc, char* argv[]) {
    Renderer renderer(800, 600);
    renderer.render();

    SDL_Delay(50000); 
    SDL_DestroyWindow(renderer.window);
    SDL_Quit();
    return 0;
}
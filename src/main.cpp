#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "renderer.hpp"

int main(int argc, char* argv[]) {
    Renderer renderer(800, 600);
    while (renderer.is_running) {
        renderer.process_input();
        renderer.update();
        renderer.render();
    }

    SDL_DestroyWindow(renderer.window);
    SDL_Quit();
    return 0;
}
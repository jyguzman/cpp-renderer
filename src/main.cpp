#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "renderer.hpp"
bool is_running = false;void process_input(Renderer* renderer) {}void update(Renderer* renderer) {}void render(Renderer* renderer) {
    SDL_RenderClear(renderer->renderer);
    renderer->draw_line(10, 10, 100, 100, 0xff0000ff);
    renderer->render_color_buffer();
    renderer->clear_color_buffer(0xff0000ff);
    SDL_RenderPresent(renderer->renderer);
}

int main(int argc, char* argv[]) {
    Renderer renderer(800, 600);
    render(&renderer);

    SDL_Delay(50000); 
    SDL_DestroyWindow(renderer.window);
    SDL_Quit();
    return 0;
}
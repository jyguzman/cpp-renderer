#include <SDL3/SDL.h>
#include <iostream>
#include "renderer.hpp"
#include "geometry.hpp"

Renderer::Renderer(int window_width, int window_height) {
    this->window_width = window_width;
    this->window_height = window_height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        this->is_running = false;
    }

    this->window = SDL_CreateWindow("3D Renderer", window_width, window_height, SDL_WINDOW_RESIZABLE);
    if (!this->window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        this->is_running = false;
    }

    this->renderer = SDL_CreateRenderer(window, nullptr);
    if (!this->renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        this->is_running = false;
    }

    this->color_buffer.resize(window_width * window_height);
    this->is_running = true;
};

void Renderer::render() {
    SDL_RenderClear(this->renderer);
    this->draw_line(10, 10, 100, 100, 0xff0000ff);
    this->render_color_buffer();
    SDL_RenderPresent(this->renderer);
}

void Renderer::update() {

}

void Renderer::process_input() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            this->is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_ESCAPE) this->is_running = false;
            break;
        }
    }

}

void Renderer::set_target_fps(double fps) {

}

void Renderer::draw_rect(int x, int y, int width, int height, uint32_t color) {
    if (x < 0 || x >= width) {
        std::cerr << "draw_rect: " << x << "is out of bounds for width " << width << ".";
        return;
    }

    if (y < 0 || y >= height) {
        std::cerr << "draw_rect: " << y << "is out of bounds for height " << width << ".";
        return;
    }

    for (int i = y; i < height; i++) {
        for (int j = x; j < width; j++) {
            this->set_color(color, i, j);
        }
    }
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int side_len = abs(delta_x) > abs(delta_y) ? abs(delta_x) : abs(delta_y);

    double x_inc = delta_x / (double)side_len;
    double y_inc = delta_y / (double)side_len;

    double curr_x = x0;
    double curr_y = y0;

    for (int i = 0; i < side_len; i++) {
        this->set_color(color, round(curr_x), round(curr_y));
        curr_x += x_inc;
        curr_y += y_inc;
    }
}

void Renderer::draw_mesh(Mesh* mesh) {

    return;
}

void Renderer::set_color(uint32_t color, int x, int y) {
    this->color_buffer[(this->window_width * x) + y] = color;
}

void Renderer::render_color_buffer() {
    SDL_Texture* texture = SDL_CreateTexture(this->renderer, 
        SDL_PIXELFORMAT_ABGR8888, 
        SDL_TEXTUREACCESS_STREAMING,
        this->window_width, this->window_height);
    SDL_UpdateTexture(texture, nullptr, this->color_buffer.data(), 
        (int)(this->window_width * sizeof(uint32_t)));
    SDL_RenderTexture(this->renderer, texture, nullptr, nullptr);
}

void Renderer::clear_color_buffer(uint32_t color) {
    for (int i = 0; i < this->window_height; i++) {
        for (int j = 0; j < this->window_width; j++) {
            this->set_color(color, i, j);
        }
    }
}
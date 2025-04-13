#include <SDL3/SDL.h>
#include <iostream>
#include "renderer.hpp"
#include "geometry.hpp"

Vec3 transform(Vec3 v, Vec3 rotation) {
    return v
        .rotate_x(rotation.x).rotate_y(rotation.y).rotate_z(rotation.z)
        .translate(0, 0, 5)
        .project(256);
}

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

Mesh cube(
    {
         Vec3 { -1, -1, -1 },
         Vec3 { -1, 1, -1 },
         Vec3 { 1, 1, -1 },
         Vec3 { 1, -1, -1 },
         Vec3 { 1, 1, 1 },
         Vec3 { 1, -1, 1 },
         Vec3 { -1, 1, 1 },
         Vec3 { -1, -1, 1 },
    },
    {
         Face { 1, 2, 3 },
         Face { 1, 3, 4 },
         Face { 4, 3, 5 },
         Face { 4, 5, 6 },
         Face { 6, 5, 7 },
         Face { 6, 7, 8 },
         Face { 8, 2, 1 },
         Face { 2, 7, 5 },
         Face { 2, 5, 3 },
         Face { 6, 8, 1 },
         Face { 6, 1, 4 },
    },
    Vec3{ 0, 0, 0 }
    );

void Renderer::update() {

}

void Renderer::render() {
    SDL_RenderClear(this->renderer);
    this->clear_color_buffer(0);
    this->draw_mesh(&cube);
    this->render_color_buffer();
    SDL_RenderPresent(this->renderer);
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
    if (x < 0 || x >= this->window_width) {
        std::cerr << "draw_rect: " << x << "is out of bounds for window width " << this->window_width << ".";
        return;
    }

    if (y < 0 || y >= this->window_height) {
        std::cerr << "draw_rect: " << y << "is out of bounds for window height " << this->window_height << ".";
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
    auto vertices = mesh->vertices;
    auto faces = mesh->faces;

    // Remove later
    double w = this->window_width / 2;
    double h = this->window_height / 2;
    mesh->rotation = mesh->rotation.translate(0.01, 0.01, 0.01);

    for (int i = 0; i < faces.size(); i++) {
        Face f = faces[i];

        // Remove '-1' after loading meshes
        Vec3 v1 = vertices[f.x - 1];
        Vec3 v2 = vertices[f.y - 1];
        Vec3 v3 = vertices[f.z - 1];

        Vec3 t1 = transform(v1, mesh->rotation).translate(w, h, 0);
        Vec3 t2 = transform(v2, mesh->rotation).translate(w, h, 0);
        Vec3 t3 = transform(v3, mesh->rotation).translate(w, h, 0);

        this->draw_line(t1.x, t1.y, t2.x, t2.y, 0xffffffff);
        this->draw_line(t1.x, t1.y, t3.x, t3.y, 0xffffffff);
        this->draw_line(t2.x, t2.y, t3.x, t3.y, 0xffffffff);

        this->draw_rect(t1.x, t1.y, 100, 100, 0xffffffff);
        this->draw_rect(t2.x, t2.y, 100, 100, 0xffffffff);
        this->draw_rect(t3.x, t3.y, 100, 100, 0xffffffff);

    }
}

void Renderer::set_color(uint32_t color, int x, int y) {
    if (x < 0 || x >= this->window_width) {
        std::cerr << "draw_rect: " << x << "is out of bounds for window width " << this->window_width << ".";
        return;
    }

    if (y < 0 || y >= this->window_height) {
        std::cerr << "draw_rect: " << y << "is out of bounds for window height " << this->window_height << ".";
        return;
    }

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
    SDL_DestroyTexture(texture);
}

void Renderer::clear_color_buffer(uint32_t color) {
    for (int i = 0; i < this->window_width * this->window_height; i++) {
        this->color_buffer[i] = color;
    }
}
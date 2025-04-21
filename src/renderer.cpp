#include <SDL3/SDL.h>
#include <iostream>
#include "include/renderer.hpp"
#include "include/geometry.hpp"
#include "include/obj.hpp"

static void int_swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static Vec2 triangle_midpoint(int x1, int y1, int x2, int y2, int x3, int y3) {
    float mid_x = (((x3 - x1) * (y2 - y1)) / (y3 - y1)) + x1;
    return Vec2(mid_x, y2);
}

void Renderer::draw_triangle_flat_bottom(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    float inv_slope_1 = (float)(x2 - x1) / (y2 - y1);
    float inv_slope_2 = (float)(x3 - x1) / (y3 - y1);

    float start_x = x1;
    float end_x = x1;

    for (int y = y1; y <= y3; ++y) {
        draw_line(start_x, y, end_x, y, color);
        start_x += inv_slope_1;
        end_x += inv_slope_2;
    }
}

void Renderer::draw_triangle_flat_top(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    float inv_slope_1 = (float)(x3 - x1) / (y3 - y1);
    float inv_slope_2 = (float)(x3 - x2) / (y3 - y2);

    float start_x = x3;
    float end_x = x3;

    for (int y = y3; y >= y1; --y) {
        draw_line(start_x, y, end_x, y, color);
        start_x -= inv_slope_1;
        end_x -= inv_slope_2;
    }
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

    this->texture = SDL_CreateTexture(this->renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        this->window_width, this->window_height);
    this->mesh = load_obj("../../../assets/cube.obj");
    this->triangles.resize(0);
    this->should_cull = true;
    this->color_buffer.resize(window_width * window_height);
    this->is_running = true;
}

void Renderer::update() {
    this->draw_mesh(&this->mesh);
}

void Renderer::render() {
    SDL_RenderClear(this->renderer);
    this->clear_color_buffer(0);
    for (auto &points: this->triangles) {
        Vec2 p1 = points.at(0);
        Vec2 p2 = points.at(1);
        Vec2 p3 = points.at(2);
        this->draw_filled_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0xffffffff);
        this->draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0xFF000000);
    }
    this->triangles.clear();
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
            switch (event.key.key) {
            case SDLK_ESCAPE:
                this->is_running = false;
                break;
            case SDLK_C:
                this->should_cull = true;
                break;
            case SDLK_D:
                this->should_cull = false;
                break;
            }
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

void Renderer::draw_filled_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if (y2 > y3) {
        int_swap(&y2, &y3);
        int_swap(&x2, &x3);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }

    if (y2 == y3) {
        this->draw_triangle_flat_bottom(x1, y1, x2, y2, x3, y3, color);
    } else if (y1 == y2) {
        this->draw_triangle_flat_top(x1, y1, x2, y2, x3, y3, color);
    } else {
        int My = y2;
        int Mx = (((x3 - x1) * (y2 - y1)) / (y3 - y1)) + x1;
        this->draw_triangle_flat_bottom(x1, y1, x2, y2, Mx, My, color);
        this->draw_triangle_flat_top(x2, y2, Mx, My, x3, y3, color);
    }
}

void Renderer::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    this->draw_line(x1, y1, x2, y2, color);
    this->draw_line(x2, y2, x3, y3, color);
    this->draw_line(x3, y3, x1, y1, color);
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int side_len = abs(delta_x) > abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_len;
    float y_inc = delta_y / (float)side_len;

    float curr_x = x0;
    float curr_y = y0;

    for (int i = 0; i <= side_len; i++) {
        int x = round(curr_x);
        int y = round(curr_y);
        this->set_color(color, x, y);
        curr_x += x_inc;
        curr_y += y_inc;
    }
}

static Vec3 transform(Vec3 v, Vec3 rotation) {
    Vec3 t = v
        .rotate_x(rotation.x)
        .rotate_y(rotation.y)
        .rotate_z(rotation.z)
        .translate(Vec3(0, 0, 5));
    return t;
}

void Renderer::draw_mesh(Mesh* mesh) {
    auto& vertices = mesh->vertices;
    auto& faces = mesh->faces;
    mesh->rotation = mesh->rotation + Vec3(0.001, 0.001, 0.001);

    for (auto& f : faces) {
        Vec3 v1 = vertices[f.x - 1];
        Vec3 v2 = vertices[f.y - 1];  
        Vec3 v3 = vertices[f.z - 1]; 

        Vec3 t1 = transform(v1, mesh->rotation);
        Vec3 t2 = transform(v2, mesh->rotation);
        Vec3 t3 = transform(v3, mesh->rotation);

        if (this->should_cull) {
            Vec3 a = (t2 - t1).normalize();
            Vec3 b = (t3 - t1).normalize();
            Vec3 normal = a.cross(b).normalize();
            Vec3 camera = Vec3(0, 0, 0);
            Vec3 camera_ray = camera - t1;
            double dot = normal.dot(camera_ray);
            if (dot < 0) {
                continue;
            }
        }

        float w = this->window_width / 2.0;
        float h = this->window_height / 2.0;
        float fov_factor = 256;

        Vec2 p1 = Vec2((fov_factor * t1.x) / t1.z, (fov_factor * t1.y) / t1.z) + Vec2(w, h);
        Vec2 p2 = Vec2((fov_factor * t2.x) / t2.z, (fov_factor * t2.y) / t2.z) + Vec2(w, h);
        Vec2 p3 = Vec2((fov_factor * t3.x) / t3.z, (fov_factor * t3.y) / t3.z) + Vec2(w, h);
        
        this->triangles.push_back({ p1, p2, p3 });
    }
}

void Renderer::set_color(uint32_t color, int x, int y) {
    if (x < 0 || x >= this->window_width) {
        std::cerr << "set_color: " << x << "is out of bounds for window width " << this->window_width << ".\n";
        return;
    }

    if (y < 0 || y >= this->window_height) {
        std::cerr << "set_color: " << y << "is out of bounds for window height " << this->window_height << ".\n";
        return;
    }

    this->color_buffer[(this->window_width * y) + x] = color;
}

void Renderer::render_color_buffer() {
    SDL_UpdateTexture(texture, nullptr, this->color_buffer.data(), 
        (int)(this->window_width * sizeof(uint32_t)));
    SDL_RenderTexture(this->renderer, texture, nullptr, nullptr);
}

void Renderer::clear_color_buffer(uint32_t color) {
    for (int y = 0; y < this->window_height; y++) {
        for (int x = 0; x < this->window_width; x++) {
            this->color_buffer[(this->window_width * y) + x] = color;
        }
    }
}
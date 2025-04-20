#include <SDL3/SDL.h>
#include <iostream>
#include "include/renderer.hpp"
#include "include/geometry.hpp"
#include "include/obj.hpp"

static bool vec2_sort(Vec2 const& lhs, Vec2 const& rhs) {
    return lhs.y < rhs.y;
}

static Vec2 triangle_midpoint(Vec2 p1, Vec2 p2, Vec2 p3) {
    float y1 = p1.y;
    float y2 = p2.y;
    float y3 = p3.y;
    float x1 = p1.x;
    float x3 = p3.x;

    float mid_x = x1 + (x3 - x1) * ((y2 - y1) / (y3 - y1));
    
    return Vec2(mid_x, y2);
}

void Renderer::draw_triangle_flat_top(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color) {
    float a = p2.y - p1.y;
    float b = p3.y - p1.y;
    float inv_slope_one = (p3.x - p1.x) / (p3.y - p1.y);
    float inv_slope_two = (p3.x - p2.x) / (p3.y - p2.y);
    float start_x = p3.x;
    float end_x = p3.x;
    for (int y = (int)p3.y; y >= (int)p1.y; y--) {
        this->draw_line(start_x, y, end_x, y, color);
        start_x -= inv_slope_one;
        end_x -= inv_slope_two;
    }
}

void Renderer::draw_triangle_flat_bottom(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color) {
    float denom1 = p2.y - p1.y;
    float denom2 = p3.y - p1.y;

    float inv_slope_one = (p2.x - p1.x) / denom1;
    float inv_slope_two = (p3.x - p1.x) / denom2;
    float start_x = p1.x;
    float end_x = p1.x;
    for (int y = p1.y; y <= p3.y; ++y) {
        this->draw_line(start_x, y, end_x, y, color);
        start_x += inv_slope_one;
        end_x += inv_slope_two;
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

    this->should_cull = true;
    this->color_buffer.resize(window_width * window_height);
    this->is_running = true;
}

Mesh mesh = load_obj("../../../assets/cube.obj");

void Renderer::update() {

}

void Renderer::render() {
    SDL_RenderClear(this->renderer);
    this->clear_color_buffer(0);
    this->draw_mesh(&mesh);
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

void Renderer::draw_triangle(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color) {
    this->draw_line(p1.x, p1.y, p2.x, p2.y, color);
    this->draw_line(p2.x, p2.y, p3.x, p3.y, color);
    this->draw_line(p3.x, p3.y, p1.x, p1.y, color);
    std::vector<Vec2> points = { p1, p2, p3 };
    std::sort(points.begin(), points.end(), &vec2_sort);
    if (points.at(1).y == points.at(2).y) {
        this->draw_triangle_flat_bottom(points.at(0), points.at(1), points.at(2), color);
    } else if (points.at(0).y == points.at(1).y) {
        this->draw_triangle_flat_top(points.at(0), points.at(1), points.at(2), color);
    } else {
        Vec2 mp = triangle_midpoint(points.at(0), points.at(1), points.at(2));
        this->draw_triangle_flat_bottom(points.at(0), points.at(1), mp, color);
        this->draw_triangle_flat_top(points.at(1), mp, points.at(2), color);
    }
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int side_len = abs(delta_x) > abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_len;
    float y_inc = delta_y / (float)side_len;

    float curr_x = x0;
    float curr_y = y0;

    for (int i = 0; i < side_len; i++) {
        int x = round(curr_x);
        int y = round(curr_y);
        //this->set_color(color, round(curr_x), round(curr_y));
        if (x >= 0 && x < this->window_width && y >= 0 && y < this->window_height) {
            this->set_color(color, x, y);
        }
        curr_x += x_inc;
        curr_y += y_inc;
    }
}

static Vec3 transform(Vec3 v, Vec3 rotation) {
    return v
        .rotate_x(rotation.x)
        .rotate_y(rotation.y)
        .rotate_z(rotation.z)
        .translate(Vec3(0, 0, 5));
}

void Renderer::draw_mesh(Mesh* mesh) {
    auto& vertices = mesh->vertices;
    auto& faces = mesh->faces;

    mesh->rotation = mesh->rotation.translate(Vec3(0.001, 0.001, 0.001));

    for (int i = 0; i < faces.size(); i++) {
        Face f = faces[i];

        Vec3 v1 = vertices[f.x];
        Vec3 v2 = vertices[f.y];
        Vec3 v3 = vertices[f.z]; 

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

        Vec2 p1 = t1.project(256).translate(Vec3(w, h, 0)).to_vec2();
        Vec2 p2 = t2.project(256).translate(Vec3(w, h, 0)).to_vec2();
        Vec2 p3 = t3.project(256).translate(Vec3(w, h, 0)).to_vec2();

        this->draw_triangle(p1, p2, p3, 0xffffffff);
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
    for (int y = 0; y < this->window_height; y++) {
        for (int x = 0; x < this->window_width; x++) {
            this->color_buffer[(this->window_width * y) + x] = color;
        }
    }
}
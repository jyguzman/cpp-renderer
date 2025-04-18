#ifndef RENDERER_HPP 
#define RENDERER_HPP

#include <SDL3/SDL.h>
#include <vector>
#include "geometry.hpp"

struct Renderer {
	bool is_running;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int window_width, window_height;
	std::vector<uint32_t> color_buffer;
	bool should_cull;

	Renderer(int window_width, int window_height);
	void render();
	void update();
	void process_input();
	void set_target_fps(double fps);
	void draw_triangle(Vec3 v1, Vec3 v2, Vec3 v3, uint32_t color);
	void draw_rect(int x, int y, int width, int height, uint32_t color);
	void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
	void draw_mesh(Mesh* mesh);
	void set_color(uint32_t, int x, int y);
	void clear_color_buffer(uint32_t color);
	void render_color_buffer();
};

#endif
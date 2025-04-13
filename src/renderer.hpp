#ifndef RENDERER_HPP 
#define RENDERER_HPP

#include <SDL3/SDL.h>
#include <vector>
#include "geometry.hpp"

struct Renderer {
	SDL_Window* window;
	int window_width, window_height;
	SDL_Renderer* renderer;
	std::vector<uint32_t> color_buffer;

	Renderer(int window_width, int window_height);
	void draw_rect(int, int, int, int, uint32_t);
	void draw_line(int, int, int, int, uint32_t);
	void draw_mesh(Mesh* mesh);
	void set_color(uint32_t, int, int);
	void clear_color_buffer(uint32_t);
	void render_color_buffer();
};

#endif
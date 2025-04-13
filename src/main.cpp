#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <filesystem>
#include "renderer.hpp"
#include "math.hpp"
#include "obj.h"

int main(int argc, char* argv[]) { /*
    auto cwd = std::filesystem::current_path();
    auto rel = std::filesystem::relative(cwd, "assets");
    std::cout << cwd << "\n" << rel;*/
    /*auto path = "../../../assets/lion.obj";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << path;
        return 1;
    }*/
   /* Mesh lion = load_obj("../../../assets/lion.obj");
    if (lion.empty()) std::cout << "empty";*/
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
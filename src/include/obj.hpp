#ifndef OBJ_HPP 
#define OBJ_HPP

#include "geometry.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <vector>

std::vector<std::string> split_by_char(const std::string& str, char c);
Mesh load_obj(const std::string);

#endif
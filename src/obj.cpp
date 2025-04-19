#include "include/obj.hpp"
#include "include/geometry.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <future>

std::vector<std::string> split_by_char(const std::string& str, char c) {
	std::vector<std::string> str_split;
	std::string current_str = "";
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == c) {
			if (current_str.size() > 0) {
				str_split.push_back(current_str);
				current_str.clear();
			}
			continue;
		}
		current_str += str[i];
	}
	if (current_str.size() > 0) str_split.push_back(current_str);
	return str_split;
}

// Only parse "v" and "f" lines for OBJ. Assumes trinagular faces
Mesh load_obj(const std::string path) {
	Mesh obj({}, {}, Vec3(0, 0, 0));
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open OBJ file: " << path;
		return obj;
	}
	
	for (std::string line; std::getline(file, line);) {
		std::vector<std::string> parts = split_by_char(line, ' ');
		if (parts.size() < 4) continue;
		if (parts[0] == "v") {
			float x = (float)std::atof(parts[1].data());
			float y = (float)std::atof(parts[2].data());
			float z = (float)std::atof(parts[3].data());
			obj.vertices.push_back(Vec3{ x, y, z });
			continue;
		}
		if (parts[0] == "f") {
			std::string f1 = split_by_char(parts[1], '/')[0];
			std::string f2 = split_by_char(parts[2], '/')[0];
			std::string f3 = split_by_char(parts[3], '/')[0];
			int x = std::atoi(f1.data()) - 1;
			int y = std::atoi(f2.data()) - 1;
			int z = std::atoi(f3.data()) - 1;
			obj.faces.push_back(Face{ x, y, z });
		}
	}

	file.close();
	return obj;
}
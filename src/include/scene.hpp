#ifndef SCENE_HPP 
#define SCENE_HPP

#include "math.hpp"
#include "geometry.hpp"
#include <vector>

struct Camera {
	Vec3 position;
	Vec3 rotation;
	float fov_angle;
};

struct Node {
	Mesh* mesh;
	std::vector<Node> children;
};

#endif
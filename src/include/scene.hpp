#ifndef SCENE_HPP 
#define SCENE_HPP

#include "math.hpp"

struct Camera {
	Vec3 position;
	Vec3 rotation;
	float fov_angle;
};

#endif
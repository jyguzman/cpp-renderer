#ifndef GEOMETRY_HPP 
#define GEOMETRY_HPP

#include "math.hpp"
#include <vector>

struct Face {
    double x, y, z;
};

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<Face> faces;
    Vec3 rotation;
};

#endif 
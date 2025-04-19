#ifndef GEOMETRY_HPP 
#define GEOMETRY_HPP

#include "math.hpp"
#include <vector>

struct Face {
    int x, y, z;
};

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<Face> faces;
    Vec3 rotation;

    Mesh();
    Mesh(std::vector<Vec3> vertices, std::vector<Face> faces, Vec3 rotation);
    bool empty() const;
};

#endif 
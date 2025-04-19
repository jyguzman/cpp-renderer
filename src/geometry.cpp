#include "include/geometry.hpp"
#include "include/math.hpp"

Mesh::Mesh() {
    this->vertices = {};
    this->faces = {};
    this->rotation = Vec3(0,0,0);
}

Mesh::Mesh(std::vector<Vec3> vertices, std::vector<Face> faces, Vec3 rotation) {
    this->vertices = vertices;
    this->faces = faces;
    this->rotation = rotation;
}

bool Mesh::empty() const {
	return this->vertices.empty() || this->faces.empty();
}
#include "include/geometry.hpp"

bool Mesh::empty() {
	return this->vertices.empty() || this->faces.empty();
}
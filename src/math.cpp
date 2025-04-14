#include "include/math.hpp";
#include <corecrt_math.h>

Vec3 Vec3::translate(double x, double y, double z) {
	return Vec3{ this->x + x, this->y + y, this->z + z };
}

double Vec3::magnitude() {
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::operator+(Vec3* other) {
	return Vec3{ this->x + other->x, this->y + other->y, this->z + other->z };
}

Vec3 Vec3::operator-(Vec3* other) {
	return Vec3{ this->x - other->x, this->y - other->y, this->z - other->z };
}

Vec3 Vec3::scale(double x) {
	return Vec3{ x * this->x, x * this->y, x*this->z };
}

double Vec3::dot(Vec3* other) {
	return this->x*other->x + this->y*other->y + this->z*other->z;
}

Vec3 Vec3::cross(Vec3* other) {
	return Vec3{this->y*other->z - this->z*other->y, 
				this->z*other->x - this->x*other->z,
				this->x*other->y - this->y*other->x};
}

Vec3 Vec3::project(double factor) {
	double z = this->z < 0.0001 ? 0.0001 : this->z; 
	return Vec3{ factor * this->x / z, factor * this->y / z, 0 };
}

Vec3 Vec3::rotate_x(double angle) {
	double y = this->y * cos(angle) - this->z * sin(angle);
	double z = this->y * sin(angle) + this->z * cos(angle);
	return Vec3{ this->x, y, z };
}

Vec3 Vec3::rotate_y(double angle) {
	double x = this->x * cos(angle) - this->z * sin(angle);
	double z = this->x * sin(angle) + this->z * cos(angle);
	return Vec3{ x, this->y, z };
}

Vec3 Vec3::rotate_z(double angle) {
	double x = this->x * cos(angle) - this->y * sin(angle);
	double y = this->x * sin(angle) + this->y * cos(angle);
	return Vec3{ x, y, this->z };
}

Mat4::Mat4(double data[16]) {
	int k = 0;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			this->data[i][j] = data[k++];
		}
	}
}

double Mat4::at(int row, int col) const {
	return this->data[row][col];
}
#include "include/math.hpp"
#include <corecrt_math.h>

Vec2::Vec2() {
	this->x = 0;
	this->y = 0;
}

Vec2::Vec2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vec2 Vec2::translate(const Vec2& other) {
	return Vec2(this->x + other.x, this->y + other.y);
}

float Vec2::magnitude() const {
	return (float)sqrt(this->x * this->x + this->y * this->y);
}

Vec2 Vec2::operator+(const Vec2& other) {
	return Vec2(this->x + other.x, this->y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) {
	return Vec2(this->x - other.x, this->y - other.y);
}

Vec2 Vec2::scale(float x) {
	return Vec2(x * this->x, x * this->y);
}

float Vec2::dot(const Vec2& other) const {
	return this->x * other.x + this->y * other.y;
}

Vec2 Vec2::normalize() {
	float magnitude = this->magnitude();
	return Vec2(this->x / magnitude, this->y / magnitude);
}

Vec3::Vec3() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vec3::Vec3(float x, float y, float z) {
	this->x = x; 
	this->y = y;
	this->z = z;
}

Vec3 Vec3::translate(const Vec3& other) {
	return Vec3{ this->x + other.x, this->y + other.y, this->z + other.z };
}

float Vec3::magnitude() const {
	return (float)sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::operator+(const Vec3& other) {
	return Vec3{ this->x + other.x, this->y + other.y, this->z + other.z };
}

Vec3 Vec3::operator-(const Vec3& other) {
	return Vec3{ this->x - other.x, this->y - other.y, this->z - other.z };
}

Vec3 Vec3::scale(float x) {
	return Vec3{ x * this->x, x * this->y, x*this->z };
}

float Vec3::dot(const Vec3& other) const {
	return this->x*other.x + this->y*other.y + this->z*other.z;
}

Vec3 Vec3::cross(const Vec3& other) {
	return Vec3{this->y*other.z - this->z*other.y, 
				this->z*other.x - this->x*other.z,
				this->x*other.y - this->y*other.x};
}

Vec3 Vec3::normalize() {
	float magnitude = this->magnitude();
	return Vec3{ this->x / magnitude, this->y / magnitude, this->z / magnitude };
}

Vec3 Vec3::project(float factor) {
	//float z = this->z < (float)0.0001 ? (float)0.0001 : this->z; 
	return Vec3(factor * this->x / z, factor * this->y / z, 0);
}

Vec3 Vec3::rotate_x(float angle) {
	float y = this->y * cos(angle) - this->z * sin(angle);
	float z = this->y * sin(angle) + this->z * cos(angle);
	return Vec3(this->x, y, z);
}

Vec3 Vec3::rotate_y(float angle) {
	float x = this->x * cos(angle) - this->z * sin(angle);
	float z = this->x * sin(angle) + this->z * cos(angle);
	return Vec3(x, this->y, z);
}

Vec3 Vec3::rotate_z(float angle) {
	float x = this->x * cos(angle) - this->y * sin(angle);
	float y = this->x * sin(angle) + this->y * cos(angle);
	return Vec3(x, y, this->z);
}

Vec2 Vec3::to_vec2() {
	return Vec2(this->x, this->y);
}

Mat4::Mat4() {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			this->data[i][j] = 0;
		}
	}
}

Mat4::Mat4(float data[4][4]) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			this->data[i][j] = data[i][j];
		}
	}
}

float Mat4::at(int row, int col) const {
	return this->data[row][col];
}

Mat4 Mat4_Identity() {
	float eye[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	return Mat4(eye);
}
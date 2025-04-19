#ifndef MATH_HPP 
#define MATH_HPP

struct Vec2 {
    float x, y;

    Vec2();
    Vec2(float x, float y);
    Vec2 translate(const Vec2& other);
    Vec2 operator+(const Vec2& other);
    Vec2 operator-(const Vec2& other);
    float magnitude() const;
    Vec2 scale(float factor);
    float dot(const Vec2& other) const;
    Vec2 normalize();
    Vec2 rotate_x(float angle);
    Vec2 rotate_y(float angle);
};

struct Vec3 {
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);
    Vec3 translate(const Vec3& other);
    Vec3 operator+(const Vec3& other);
    Vec3 operator-(const Vec3& other);
    float magnitude() const;
    Vec3 scale(float factor);
    float dot(const Vec3& other) const;
    Vec3 cross(const Vec3& other);
    Vec3 normalize();
    Vec3 project(float factor);
    Vec3 rotate_x(float angle);
    Vec3 rotate_y(float angle);
    Vec3 rotate_z(float angle);
};

struct Mat4 {
    float data[4][4];
    Mat4(float data[16]);
    float at(int row, int col) const;
};

#endif 
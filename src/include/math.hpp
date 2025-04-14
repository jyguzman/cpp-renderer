#ifndef MATH_HPP 
#define MATH_HPP

struct Vec3 {
    double x, y, z;

    Vec3 translate(double x, double y, double z);
    Vec3 operator+(Vec3* other);
    Vec3 operator-(Vec3* other);
    double magnitude();
    Vec3 scale(double factor);
    double dot(Vec3* other);
    Vec3 cross(Vec3* other);
    Vec3 project(double factor);
    Vec3 rotate_x(double angle);
    Vec3 rotate_y(double angle);
    Vec3 rotate_z(double angle);
};

struct Mat4 {
    double data[4][4];
    Mat4(double data[16]);
    double at(int row, int col) const;
};

#endif 
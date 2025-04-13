#ifndef MATH_HPP 
#define MATH_HPP

struct Vec3 {
    double x, y, z;

    Vec3 translate(double x, double y, double z);
    Vec3 scale(double factor);
    Vec3 dot(Vec3*);
    Vec3 project(double factor);
    Vec3 rotate_x(double angle);
    Vec3 rotate_y(double angle);
    Vec3 rotate_z(double angle);
};

#endif 
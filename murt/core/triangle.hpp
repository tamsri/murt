#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.hpp"
#include "ray.hpp"

class Triangle
{
public:
    Vec3 p1_, p2_, p3_;

    Triangle(Vec3 p1, Vec3 p2, Vec3 p3) : p1_(p1), p2_(p2), p3_(p3){};

    bool IsIntersect(const Ray ray, float &distance)
    {

        return false;
    }
};

#endif // TRIANGLE_H
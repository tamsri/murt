#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.hpp"
#include "ray.hpp"

class Triangle
{
public:
    Vec3 p1_, p2_, p3_;
    Vec3 normal_;
    Triangle(Vec3 p1, Vec3 p2, Vec3 p3) : p1_(p1), p2_(p2), p3_(p3)
    {
        Vec3 a = p2_ - p1_;
        Vec3 b = p3_ - p1_;
        normal_ = Vec3::Cross(a, b);
        normal_.Normalize();
    };

    bool IsIntersect(Ray ray, float &distance)
    {
        // Tomas Möller and Ben Trumbore - Fast, Minimum Storage Ray-Triangle Intersection
        constexpr float EPSIL = 0.000001f;
        // Variables
        Vec3 edge1, edge2, h, s, q;
        float a, f, u, v;

        // Vertices of Triangle
        edge1 = p2_ - p1_;
        edge2 = p3_ - p1_;

        h = Vec3::Cross(ray.direction_, edge2);
        a = Vec3::Dot(edge1, h);
        // Parallel
        if (a > -EPSIL && a < EPSIL)
            return false;

        f = 1.0f / a;
        s = ray.origin_ - p1_;
        u = f * Vec3::Dot(s, h);
        if (u < 0.0f || u > 1.0f)
            return false;

        q = Vec3::Cross(s, edge1);
        v = f * Vec3::Dot(ray.direction_, q);

        if (v < 0.0f || u + v > 1.0f)
            return false;

        distance = f * Vec3::Dot(edge2, q);
        if (distance > EPSIL)
            return true;

        return false;
    }
};

#endif // TRIANGLE_H
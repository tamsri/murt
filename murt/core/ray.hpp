#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"
#include "bvh.hpp"

class Ray
{
public:
    Vec3 origin_;
    Vec3 direction_;
    Ray(Vec3 origin, Vec3 direction) : origin_(origin), direction_(direction){};
};
#endif // RAY_Hś
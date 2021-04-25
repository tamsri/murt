#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class Ray
{
public:
    Vec3 origin_;
    Vec3 direction_;
    Vec3 in_dir_; // invert for intersection optimisation.
    Ray(Vec3 origin, Vec3 direction) : origin_(origin), direction_(direction)
    {
        direction_.Normalize(); // ensure unit vector.
        in_dir_ = Vec3(1.0f / direction_.x_, 1.0f / direction_.y_, 1.0f / direction_.z_);
    };

    Vec3 PositionAt(float distance)
    {
        return origin_ + direction_ * distance;
    }
};
#endif // RAY_Hś
#ifndef VEC3_H
#define VEC3_H
class Vec3
{
public:
    float x_, y_, z_;
    Vec3(void) : x_(0.0f), y_(0.0f), z_(0.0f){};
    Vec3(float x, float y, float z) : x_(x), y_(y), z_(z){};
};
#endif //VEC3_H
#ifndef VEC3_H
#define VEC3_H

#include <math.h>

class Vec3
{
public:
    float x_, y_, z_;
    // Constructor
    Vec3(void) : x_(0.0f), y_(0.0f), z_(0.0f){};
    Vec3(float x, float y, float z) : x_(x), y_(y), z_(z){};

    // Operators
    Vec3 operator+(const Vec3 &o_v3)
    {
        return Vec3(x_ + o_v3.x_, y_ + o_v3.y_, z_ + o_v3.z_);
    };
    Vec3 operator-(const Vec3 &o_v3)
    {
        return Vec3(x_ - o_v3.x_, y_ - o_v3.y_, z_ - o_v3.z_);
    };
    Vec3 operator*(const Vec3 &o_v3)
    {
        return Vec3(x_ * o_v3.x_, y_ * o_v3.y_, z_ * o_v3.z_);
    };
    Vec3 operator/(const Vec3 &o_v3)
    {
        return Vec3(x_ / o_v3.x_, y_ / o_v3.y_, z_ / o_v3.z_);
    };

    Vec3 operator+(const float &f)
    {
        return Vec3(x_ + f, y_ + f, z_ + f);
    };
    Vec3 operator-(const float &f)
    {
        return Vec3(x_ - f, y_ - f, z_ - f);
    };
    Vec3 operator*(const float &f)
    {
        return Vec3(x_ * f, y_ * f, z_ * f);
    };
    Vec3 operator/(const float &f)
    {
        return Vec3(x_ / f, y_ / f, z_ / f);
    };

    Vec3 &operator+=(const Vec3 &o_v3)
    {
        x_ += o_v3.x_;
        y_ += o_v3.y_;
        z_ += o_v3.z_;
        return *this;
    };
    Vec3 &operator-=(const Vec3 &o_v3)
    {
        x_ -= o_v3.x_;
        y_ -= o_v3.y_;
        z_ -= o_v3.z_;
        return *this;
    };
    Vec3 &operator*=(const Vec3 &o_v3)
    {
        x_ *= o_v3.x_;
        y_ *= o_v3.y_;
        z_ *= o_v3.z_;
        return *this;
    };
    Vec3 &operator/=(const Vec3 &o_v3)
    {
        x_ /= o_v3.x_;
        y_ /= o_v3.y_;
        z_ /= o_v3.z_;
        return *this;
    };

    Vec3 &operator+=(const float &f)
    {
        x_ += f;
        y_ += f;
        z_ += f;
        return *this;
    };
    Vec3 &operator-=(const float &f)
    {
        x_ -= f;
        y_ -= f;
        z_ -= f;
        return *this;
    };
    Vec3 &operator*=(const float &f)
    {
        x_ *= f;
        y_ *= f;
        z_ *= f;
        return *this;
    };
    Vec3 &operator/=(const float &f)
    {
        x_ /= f;
        y_ /= f;
        z_ /= f;
        return *this;
    };

    // 1. Normalize vector
    void Normalize()
    {
        float len = sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2));
        x_ /= len;
        y_ /= len;
        z_ /= len;
    };

    // 2. Calculate Cross Product
    static Vec3 Cross(Vec3 v3_1, Vec3 v3_2)
    {
        float x = v3_1.y_ * v3_2.z_ - v3_1.z_ * v3_2.y_;
        float y = v3_1.z_ * v3_2.x_ - v3_1.x_ * v3_2.z_;
        float z = v3_1.x_ * v3_2.y_ - v3_1.y_ * v3_2.x_;
        return Vec3(x, y, z);
    };

    // 3. Calculate Dot Product
    static float Dot(Vec3 v3_1, Vec3 v3_2)
    {
        return v3_1.x_ * v3_2.x_ + v3_1.y_ * v3_2.y_ + v3_1.z_ * v3_2.z_;
    };

    // 3. Return Absolute Value
    static float Abs(Vec3 v3)
    {
        return sqrt(Vec3::Dot(v3, v3));
    };

    // 4. Return Distance
    static float Distance(Vec3 v3_1, Vec3 v3_2)
    {
        return sqrt(pow(v3_1.x_ - v3_2.x_, 2) +
                    pow(v3_1.y_ - v3_2.y_, 2) +
                    pow(v3_1.z_ - v3_2.z_, 2));
    };

    // 5. Calcuclate Angle between vector direction
    static float Angle(Vec3 v3_1, Vec3 v3_2)
    {
        float abs_1 = Vec3::Abs(v3_1);
        float abs_2 = Vec3::Abs(v3_2);
        float cos_theta = Vec3::Dot(v3_1, v3_2) / (abs_1 + abs_2);
        return acos(cos_theta);
    };
};
#endif //VEC3_H
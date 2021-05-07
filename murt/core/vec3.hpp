#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <set>
#include <utility>
#include <string>

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
    bool operator==(const Vec3 &b) const
    {
        return (x_ == b.x_) && (y_ == b.y_) && (z_ == b.z_);
    }
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
        return (v3_1.x_ * v3_2.x_) + (v3_1.y_ * v3_2.y_) + (v3_1.z_ * v3_2.z_);
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
        return acos(Vec3::Dot(v3_1, v3_2) / (Vec3::Abs(v3_1) * Vec3::Abs(v3_2)));
    };

    // 6.5 check the given position is between xz plane
    static bool BetweenXZ(float min_x, float max_x, float min_z, float max_z, Vec3 pos)
    {
        return (pos.x_ >= min_x) && (pos.x_ <= max_x) && (pos.z_ >= min_z) && (pos.z_ <= max_z);
    }

    // compare operator
    bool operator<(const Vec3 &cvec) const
    {
        if (x_ != cvec.x_)
            return x_ < cvec.x_;
        else if (y_ != cvec.y_)
            return y_ < cvec.y_;
        else
            return z_ < cvec.z_;
    }

    static void ReorderEdges(Vec3 txPos, std::vector<Vec3> &edges)
    {
        txPos.y_ = 0.0f;
        std::set<std::pair<float, Vec3> > ordered_edges;
        for (Vec3 edge : edges)
        {
            Vec3 edge_on_plane = edge;
            edge_on_plane.y_ = 0.0f;
            float distance = Vec3::Distance(txPos, edge_on_plane);
            ordered_edges.insert({distance, edge});
        }

        std::vector<Vec3> new_edges;
        for (std::pair<float, Vec3> pair_edge : ordered_edges)
            new_edges.push_back(pair_edge.second);
        edges = new_edges;
        ordered_edges.clear();
    }
    std::string GetString() const
    {
        return "(" + std::to_string(x_) + ", " + std::to_string(y_) + ", " + std::to_string(z_) + ")";
    }
};

struct Vec3Hasher
{
    std::size_t operator()(const Vec3 &v) const
    {
        return ((std::hash<float>()(v.x_) ^ (std::hash<float>()(v.y_) << 1)) >> 1) ^ (std::hash<float>()(v.z_) << 1);
    }
};
#endif //VEC3_H
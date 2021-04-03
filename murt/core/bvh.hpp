#ifndef BVH_H
#define BVH_H

#include <vector>
#include <algorithm>
#include <float.h>
#include <map>

#include "triangle.hpp"
#include "ray.hpp"

class Box
{
public:
    Vec3 min_bound_;
    Vec3 max_bound_;
    std::vector<Triangle *> members_;

    Box *left_;
    Box *right_;
    float min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
    float max_x = -FLT_MAX, max_y = -FLT_MAX, max_z = -FLT_MAX;
    Box(std::vector<Triangle *> &new_members)
    {
        // require to input members
        members_ = new_members;
        // process min/max boundary
        float min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
        float max_x = -FLT_MAX, max_y = -FLT_MAX, max_z = -FLT_MAX;
        for (Triangle *triangle : members_)
        {
            min_x = std::min({triangle->p1_.x_, triangle->p2_.x_, triangle->p3_.x_, min_x});
            max_x = std::max({triangle->p1_.x_, triangle->p2_.x_, triangle->p3_.x_, max_x});
            min_y = std::min({triangle->p1_.y_, triangle->p2_.y_, triangle->p3_.y_, min_y});
            max_y = std::max({triangle->p1_.y_, triangle->p2_.y_, triangle->p3_.y_, max_y});
            min_z = std::min({triangle->p1_.z_, triangle->p2_.z_, triangle->p3_.z_, min_z});
            max_z = std::max({triangle->p1_.z_, triangle->p2_.z_, triangle->p3_.z_, max_z});
        }
        min_bound_ = Vec3(min_x, min_y, min_z);
        printf("min_bound: %.2f %.2f %.2f\n", min_x, min_y, min_z);
        max_bound_ = Vec3(max_x, max_y, max_z);
        printf("max_bound: %.2f %.2f %.2f\n", max_x, max_y, max_z);
    }
    // TODO[]: Box-Ray intersection
    bool IsIntersect(Ray &ray)
    {
        return false;
    };
};

class BVH
{
public:
    Box *root_;
    // TODO[]: Build tree
    BVH(std::vector<Triangle *> &triangles)
    {
        root_ = new Box(triangles);
        BVH::MakeChildren(root_, 0);
    }
    ~BVH()
    {
    }
    // TODO[]: Tree intersection
    bool IsIntersect(const Ray &ray, float &distance)
    {

        return false;
    }
    // TODO[X]: Make Children
    static void MakeChildren(Box *parent, int level)
    {
        printf("depth: %d, parents members: %d \n", level, parent->members_.size());
        // Dimension of Sectorisation
        // k = 0 -> sectorise by x-axis
        // k = 1 -> sectorise by y-axis
        // k = 2 -> sectorise by z-axis
        int k = level % 3;
        std::vector<Triangle *> left_members;
        std::vector<Triangle *> right_members;
        std::map<float, Triangle *> ordered_triangles;
        switch (k)
        {
        case 0:
        {
            for (Triangle *triangle : parent->members_)
            {
                float x_tri_max = std::max({triangle->p1_.x_, triangle->p2_.x_, triangle->p3_.x_});
                ordered_triangles[x_tri_max] = triangle;
            }
        }
        break;
        case 1:
        {
            for (Triangle *triangle : parent->members_)
            {
                float y_tri_max = std::max({triangle->p1_.y_, triangle->p2_.y_, triangle->p3_.y_});
                ordered_triangles[y_tri_max] = triangle;
            }
        }
        break;
        case 2:
        {
            for (Triangle *triangle : parent->members_)
            {
                float z_tri_max = std::max({triangle->p1_.z_, triangle->p2_.z_, triangle->p3_.z_});
                ordered_triangles[z_tri_max] = triangle;
            }
        }
        break;
        }

        // push ordered triangles to children' members
        size_t half_size = ordered_triangles.size() / 2;
        size_t current_pos = 0;
        for (auto &[key, triangle] : ordered_triangles)
        {
            if (current_pos < half_size)
                left_members.push_back(triangle);
            else
                right_members.push_back(triangle);

            ++current_pos;
        }
        // make children
        level += 1;
        if (!left_members.empty())
        {
            parent->left_ = new Box(left_members);
            if (left_members.size() > 1)
                BVH::MakeChildren(parent->left_, level);
        }
        if (!right_members.empty())
        {
            parent->right_ = new Box(right_members);
            if (right_members.size() > 1)
                BVH::MakeChildren(parent->right_, level);
        }
    }
};
#endif //BVH_H
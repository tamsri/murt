#ifndef BVH_H
#define BVH_H

#include <vector>

#include "triangle.hpp"
#include "ray.hpp"

class Box
{
public:
    Vec3 min_bound;
    Vec3 max_bound;
    size_t n_members;
    std::vector<Triangle *> members;

    Box *left;
    Box *right;

    // TODO[]: Initialize box
    Box(std::vector<Triangle *> triangles, size_t n){

    };
    ~Box(){

    };

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
    }
    ~BVH()
    {
    }
    // TODO[]: Tree intersection
    bool IsIntersect(const Ray &ray, float &distance)
    {

        return false;
    }
};
#endif //BVH_H
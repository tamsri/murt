#ifndef TRACER_H
#define TRACER_H

#include <vector>

#include "triangle.hpp"
#include "bvh.hpp"

class Tracer
{
public:
    unsigned int id_;
    static unsigned int global_id;
    BVH *scene_;
    std::vector<Triangle *> triangles_;

    // TODO[]: Initialie Tracer
    Tracer(std::vector<Triangle *> &triangles) : id_(global_id++), triangles_(triangles)
    {
        scene_ = new BVH(triangles_);
    };

    ~Tracer()
    {
        delete scene_;
        for (Triangle *triangle : triangles_)
            delete triangle;
        triangles_.clear();
    };

    bool IsLOS(Vec3 txPos, Vec3 rxPos, float &distance)
    {
        Vec3 ray_dir = (rxPos - txPos);
        ray_dir.Normalize();
        Ray ray(txPos, ray_dir);
        return scene_->IsIntersect(ray, distance);
    }
    // TODO[]: Trace
    void Trace(Vec3 txPos, Vec3 rxPos){

    };
};

unsigned int Tracer::global_id = 0;

#endif
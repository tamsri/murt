#ifndef TRACER_H
#define TRACER_H

class Tracer
{
public:
    unsigned int id_;
    static unsigned int global_id;
    BVH *scene_;
    std::vector<Triangle *> triangles_;

    // TODO[]: Initialie Tracer
    Tracer(std::vector<Triangle *> &triangles) : id_(global_id++)
    {
        triangles_ = triangles;
        scene_ = new BVH(triangles);
    };

    ~Tracer()
    {
        delete scene_;
        for (Triangle *triangle : triangles_)
            delete triangle;
        triangles_.clear();
    };

    // TODO[]: Trace
    void Trace(Vec3 txPos, Vec3 rxPos){

    };
};

unsigned int Tracer::global_id = 0;

#endif
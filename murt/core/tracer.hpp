#ifndef TRACER_H
#define TRACER_H

#include <vector>

#include "triangle.hpp"
#include "bvh.hpp"

enum RecordType
{
    Direct = 0,
    Diffracted = 1,
    SingleReflected = 2
};

struct Record
{
    RecordType type;
    std::vector<Vec3> points;
};

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

    bool IsLOS(Vec3 txPos, Vec3 rxPos)
    {
        Vec3 ray_dir = (rxPos - txPos);
        ray_dir.Normalize();
        Ray ray(txPos, ray_dir);
        float distance = FLT_MAX;
        scene_->IsIntersect(ray, distance);
        return !(distance <= Vec3::Distance(txPos, rxPos));
    }

    // TODO[]: get edges
    Record GetEdges(Vec3 txPos, Vec3 rxPos)
    {
        Record record;
        record.type = RecordType::Diffracted;
        constexpr size_t max_scan = 10;
        size_t current_scan = 0;
        Vec3 left_pos = txPos;
        Vec3 right_pos = rxPos;

        while (!(IsLOS(left_pos, right_pos)) && current_scan < max_scan)
        {
            current_scan++;
        }

        return record;
    }

    // TODO[] :: add mirrored point
    Vec3 GetMirrorPoint(Vec3 pos, Triangle *triangle)
    {

        Vec3 normal = triangle->normal_;
        float b = Vec3::Dot(normal, triangle->p1);
        float t = (b - Vec3::Dot(pos, normal)) / (Vec3::Dot(normal, normal));

        Vec3 mirror_pos = pos + (normal * 2 * t);
        return mirror_pos;
    }

    // TODO[]: Trace
    std::vector<Record> Trace(Vec3 txPos, Vec3 rxPos)
    {
        std::vector<Record> records;
        // LOS/NLOS
        if (IsLOS(txPos, rxPos))
        {
            Record los_record;
            los_record.type = RecordType::Direct;
            records.push_back(los_record);
        }
        else
        {
            Record edgesRecord = GetEdges(txPos, rxPos);
            records.push_back(edgesRecord);
        }

        // Reflection
        for (auto triangle : triangles_)
        {
            Vec3 mirror_point = GetMirrorPoint(pos, triangle);
            Vec3 direction_to_rx = (rxPos - mirror_point).Normalize();
            Ray ref_ray(mirror_point, direction_to_rx);
            float distance;
            if (!(triangle->IsIntersected(ref_ray, distance)))
                continue;
            Vec3 point_on_triangle = mirror_point + direction_to_rx * (distance * 0.001f);
            if (IsLOS(txPos, point_on_triangle) && IsLOS(rxPos, point_on_triangle))
                records.push_back(Record(RecordType::SingleReflected, {point_on_triangle}));
        }

        return records;
    };
};

unsigned int Tracer::global_id = 0;

#endif
#ifndef TRACER_H
#define TRACER_H

#include <vector>

#include "triangle.hpp"
#include "bvh.hpp"

enum RecordType
{
    Direct = 0,
    Diffracted = 1,
    SingleReflected = 2,
    MirrorRecord = 3
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

    // TODO[]: Initialize Tracer
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

    // IsHit is a primitive way of hit search with brute force approach
    // the optimised method is to use BVH (scene->IsIntersect).
    bool IsHit(const Ray &ray, float &closest_distance)
    {
        bool hit_something = false;
        for (Triangle *triangle : triangles_)
        {
            float distance = FLT_MAX;
            if (triangle->IsIntersect(ray, distance))
            {
                closest_distance = std::min(distance, closest_distance);
                hit_something = true;
            }
        }
        return hit_something;
    }

    bool IsLOS(Vec3 txPos, Vec3 rxPos)
    {
        Vec3 ray_dir = (rxPos - txPos);
        ray_dir.Normalize();
        Ray ray(txPos, ray_dir);
        float distance = FLT_MAX;

        if (!scene_->IsIntersect(ray, distance))
            return true;

        // if (!IsHit(ray, distance))
        //     return true;

        bool res = !(distance < Vec3::Distance(txPos, rxPos));
        printf("hit: %.2f, distance: %.2f, res: %d\n", distance, Vec3::Distance(txPos, rxPos), res);
        return res;
    }

    // TODO[]: get edges
    bool FindEdge(Vec3 leftPos, Vec3 rightPos, Vec3 &edgePos)
    {
        constexpr float scan_step = 1.0f;

        const float min_x = std::min(leftPos.x_, rightPos.x_);
        const float max_x = std::max(leftPos.x_, rightPos.x_);
        const float min_y = std::min(leftPos.y_, rightPos.y_);
        const float max_y = std::max(leftPos.y_, rightPos.y_);
        const float min_z = std::min(leftPos.z_, rightPos.z_);
        const float max_z = std::max(leftPos.z_, rightPos.z_);

        Ray ray(leftPos, leftPos - rightPos);
        for (float angle = 0.0f; angle < 180.0f; angle += scan_step)
        {
        }

        return false;
    }

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
        float b = Vec3::Dot(normal, triangle->p2_);
        float t = (b - Vec3::Dot(pos, normal)) / (Vec3::Dot(normal, normal));

        return pos + normal * 2 * t;
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
        for (Triangle *triangle : triangles_)
        {
            Vec3 mirror_point = GetMirrorPoint(txPos, triangle);

            Vec3 direction_to_rx = (rxPos - mirror_point);
            direction_to_rx.Normalize();
            Ray ref_ray(mirror_point, direction_to_rx);
            float distance;
            if (!(triangle->IsIntersect(ref_ray, distance)))
                continue;

            Vec3 point_on_triangle = mirror_point + direction_to_rx * (distance + 0.001f);

            if (IsLOS(txPos, point_on_triangle) && IsLOS(rxPos, point_on_triangle))
            {
                Record reflect_record;
                reflect_record.type = RecordType::SingleReflected;
                reflect_record.points.push_back(point_on_triangle);
                records.push_back(reflect_record);
            }
        }

        return records;
    };
};

unsigned int Tracer::global_id = 0;

#endif
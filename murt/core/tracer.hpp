#ifndef TRACER_H
#define TRACER_H

#include <mutex>
#include <vector>
#include <math.h>
#include <unordered_map>

#include "record.hpp"
#include "vec3.hpp"
#include "triangle.hpp"
#include "bvh.hpp"

#include "calculator.hpp"

class Tracer
{
public:
    unsigned int id_;
    static unsigned int global_id;
    BVH *scene_;
    std::vector<Triangle *> triangles_;

    std::mutex mutex_;
    std::vector<std::vector<float> > volume_result_;

    Tracer(std::vector<Triangle *> &triangles) : id_(global_id++), triangles_(triangles)
    {
        scene_ = new BVH(triangles_);
        volume_result_.clear();
    };

    ~Tracer()
    {
        delete scene_;
        for (Triangle *triangle : triangles_)
            delete triangle;
        triangles_.clear();
        volume_result_.clear();
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

        bool res = !(distance < Vec3::Distance(txPos, rxPos));
        return res;
    }

    bool FindEdge(Vec3 leftPos, Vec3 rightPos, Vec3 &edgePos)
    {
        constexpr size_t max_scan = 20;
        constexpr float min_angle = 0.0017f;

        const float min_x = std::min(leftPos.x_, rightPos.x_);
        const float max_x = std::max(leftPos.x_, rightPos.x_);
        const float min_z = std::min(leftPos.z_, rightPos.z_);
        const float max_z = std::max(leftPos.z_, rightPos.z_);

        // ray from leftPos to rightPos

        Vec3 top_direction(0.0f, 1.0f, 0.0f);
        Ray upper_ray(leftPos, top_direction);
        float unused_;
        if (scene_->IsIntersect(upper_ray, unused_))
            return false;
        Ray lower_ray(leftPos, rightPos - leftPos);
        size_t current_scan = 0;

        // get nearest edge
        while (current_scan++ < max_scan &&
               Vec3::Angle(lower_ray.direction_, upper_ray.direction_) > min_angle)
        {
            Vec3 new_direction = (upper_ray.direction_ + lower_ray.direction_) / 2;

            Ray check_ray(leftPos, new_direction);
            float distance = FLT_MAX;
            if (scene_->IsIntersect(check_ray, distance) &&
                Vec3::BetweenXZ(min_x, max_x, min_z, max_z, check_ray.PositionAt(distance)))
            {
                lower_ray = check_ray;
            }
            else
            {
                upper_ray = check_ray;
            }
        }
        // Actually, it's imposible to not intersect,
        // but if it does, return false. If speed is needed, comment out.
        float d = FLT_MAX;
        if (!scene_->IsIntersect(lower_ray, d))
            return false;

        // get edge position
        Vec3 leftPosOnPlane(leftPos.x_, 0.0f, leftPos.z_);
        Vec3 rightPosOnPlane(rightPos.x_, 0.0f, rightPos.z_);
        Vec3 plane_direction = rightPosOnPlane - leftPosOnPlane;
        plane_direction.Normalize();
        float theta = Vec3::Angle(plane_direction, lower_ray.direction_);

        float x_angle = Vec3::Angle(lower_ray.direction_, upper_ray.direction_);
        float height = d * cos(theta) * tan(theta + x_angle);
        float width = d * cos(theta);
        float edge_distance = sqrt(height * height + width * width);

        edgePos = upper_ray.PositionAt(edge_distance);

        return true;
    }

    bool GetEdges(Vec3 txPos, Vec3 rxPos, Record &record)
    {
        record.type = RecordType::Diffracted;
        std::vector<Vec3> points;
        constexpr size_t max_scan = 20;
        size_t current_scan = 0;
        Vec3 left_pos = txPos;
        Vec3 right_pos = rxPos;

        while (!(IsLOS(left_pos, right_pos)))
        {
            if (current_scan++ > max_scan)
                return false;
            Vec3 edge_from_left;
            if (!FindEdge(left_pos, right_pos, edge_from_left))
                return false;
            Vec3 edge_from_right;
            if (!FindEdge(right_pos, left_pos, edge_from_right))
                return false;

            if (IsLOS(edge_from_left, edge_from_right))
            {
                if (Vec3::Distance(edge_from_left, edge_from_right) < 0.5f)
                {
                    // merge the edges since they are too near.
                    Vec3 avg_edge = (edge_from_left + edge_from_right) / 2.0f;
                    record.points.push_back(avg_edge);
                    return true;
                }
                record.points.push_back(edge_from_left);
                record.points.push_back(edge_from_right);
                return true;
            }

            record.points.push_back(edge_from_left);
            record.points.push_back(edge_from_right);
            left_pos = edge_from_left;
            right_pos = edge_from_right;
        }
        return false;
    }

    Vec3 GetMirrorPoint(Vec3 pos, Triangle *triangle)
    {
        // My invented mirror formula from high school's knowledge😎
        Vec3 normal = triangle->normal_;
        float t = (Vec3::Dot(normal, triangle->p2_) - Vec3::Dot(pos, normal)) / (Vec3::Dot(normal, normal));
        return pos + normal * 2 * t;
    }

    std::vector<Record> Trace(Vec3 txPos, Vec3 rxPos)
    {
        std::vector<Record> records;

        // Trace -> LOS/NLOS
        if (IsLOS(txPos, rxPos))
        {
            Record los_record;
            los_record.type = RecordType::Direct;
            records.push_back(los_record);
        }
        else
        {
            Record record;
            if (GetEdges(txPos, rxPos, record))
            {
                Vec3::ReorderEdges(txPos, record.points);
                records.push_back(record);
            }
        }

        // Trace -> Reflection
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

    bool IsOutdoor(Vec3 pos)
    {
        Vec3 top_pos = pos;
        top_pos.y_ = 10000.0f;
        return IsLOS(pos, top_pos);
    }

    float HitNearest(Vec3 fromPos, Vec3 toPos)
    {
        float hit_distance = FLT_MAX;
        Ray ray(fromPos, toPos - fromPos);

        if (scene_->IsIntersect(ray, hit_distance))
            return hit_distance;

        return -1.0f;
    }

    void TraceAsync(Vec3 txPos, Vec3 rxPos, float txFreq, float matPerm)
    {
        std::vector<float> result;
        result.push_back(rxPos.x_);
        result.push_back(rxPos.y_);
        result.push_back(rxPos.z_);
        if (!this->IsOutdoor(txPos) || !this->IsOutdoor(rxPos))
        {
            result.push_back(-1.0f);
        }
        else
        {
            std::vector<Record> records = this->Trace(txPos, rxPos);

            if (records.empty())
            {
                result.push_back(-1.0f);
            }
            else
            {
                result.push_back(GetTotalPathLoss(txPos, rxPos, txFreq, matPerm, records));
            }
        }

        mutex_.lock();
        this->volume_result_.push_back(result);
        mutex_.unlock();
    }
};
unsigned int Tracer::global_id = 0;

#endif //!TRACER_H
#ifndef RECORD_H
#define RECORD_H

#include <vector>
#include "vec3.hpp"

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
#endif // !RECORD_H
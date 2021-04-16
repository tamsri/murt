#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include <math.h>
#include <vector>

#include "vec3.hpp"

#define LIGHT_SPEED 299792458.0f

static float GetDelay(std::vector<Vec3> points, float speed)
{
    float total_distance = 0.0f;
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        total_distance += Vec3::Distance(points[i], points[i + 1]);
    }
    return total_distance / speed;
}

static float DirectPathLoss(Vec3 txPos, Vec3 rxPos, float txFreq)
{
    return 20.0f * log10(Vec3::Distance(txPos, rxPos)) + 20.0f * log10(txFreq) - 147.5f;
}

static float GetRefCoe(Vec3 txPos, Vec3 rxPos, Vec3 refPos, float matPerm, bool isTM)
{
    Vec3 refToTx = txPos - refPos;
    refToTx.Normalize();
    Vec3 refToRx = rxPos - refPos;
    refToRx.Normalize();

    float angle = Vec3::Angle(refToTx, refToRx) / 2.0f;

    if (isTM)
    {
    }
    else
    {
    }

    return coe;
}

static float ReflectedPathLoss(Vec3 txPos, Vec3 rxPos, Vec3 refPos, float txFreq, float matPerm)
{
    float ref_coe;
    float distance = Vec3::Distance(txPos, refPos) + Vec3::Distance(refPos, rxPos);
    return 20.0f * log10(distance) + 20.0f * log10(txFreq) - 20.0f * log10(abs(ref_coe)) - 147.55f;
}

static float DiffractedPathLoss(Vec3 txPos, Vec3 rxPos, std::vector<Vec3> edges, float frequency)
{
    float pl = 0;
    return pl;
}

static PyObject *GetDirectLoss(PyObject *self, PyObject *args)
{
    PyObject *txPos;
    PyObject *rxPos;
    float txFreq;
    if (!PyArgs_ParseTuple(args, "O O f", &txPos, &rxPos, &txFreq))
        return NULL;

    std::vector<Vec3> points;
    points.push_back(txPos);
    points.push_back(rxPos);

    return Py_BuildValue("f f", DirectPathLoss(txPos, rxPos, txFreq), GetDelay(points, LIGHT_SPEED));
}

static PyObject *GetDiffractionLoss(PyObject *self, PyObject *args)
{

    PyObject *txPosObj;
    PyObject *rxPosObj;
    PyObject *edgesObj;
    float txFreq;
    if (!PyArgs_ParseTuple(args, "O O O f", &txPosObj, &rxPosObj, &edgesObj, &txFreq))
        return NULL;

    Vec3 txPos;
    if (!PyArgs_ParseTuple(txPosObj, "f f f", &txPos.x_, &txPos.y_, &txPos.z_))
        return NULL;
    Vec3 rxPos;
    if (!PyArgs_ParseTuple(rxPosObj, "f f f", &rxPos.x_, &rxPos.y_, &rxPos.z_))
        return NULL;
    std::vector<Vec3> edges;
    for (int i = 0; i < PyList_Size(edgesObj); ++i)
    {
        PyObject *edgeValue = PyList_GetItem(edgesObj, i);
        Vec3 edge;
        if (!PyArgs_ParseTuple(edgeValue, "f f f", &edge.x_, &edge.y_, &edge.z_))
            return NULL;
        edges.push_back(edge);
    }

    std::vector<Vec3> points;
    points.push_back(txPos);
    for (Vec3 &edge : edges)
        points.push_back(edge);
    points.push_back(rxPos);

    return Py_BuildValue("f f", DiffractedPathLoss(txPos, rxPos, edges, txFreq), GetDelay(points, LIGHT_SPEED));
}

static PyObject *GetReflectionLoss(PyObject *self, PyObject *args)
{

    PyObject *txPosObj;
    PyObject *rxPosObj;
    PyObject *refPosObj;
    float txFreq;
    float matPerm;
    if (!PyArgs_ParseTuple(args, "O O O f f", &txPosObj, &rxPosObj, &refPosObj, &txFreq, &matPerm))
        return NULL;

    Vec3 txPos;
    if (!PyArgs_ParseTuple(txPosObj, "f f f", &txPos.x_, &txPos.y_, &txPos.z_))
        return NULL;
    Vec3 rxPos;
    if (!PyArgs_ParseTuple(rxPosObj, "f f f", &rxPos.x_, &rxPos.y_, &rxPos.z_))
        return NULL;
    Vec3 refPos;
    if (!PyArgs_ParseTuple(refPosObj, "f f f", &refPos.x_, &refPos.y_, &refPos.z_))
        return NULL;

    std::vector<Vec3> points;
    points.push_back(txPos);
    points.push_back(refPos);
    points.push_back(rxPos);

    return Py_BuildValue("f f", ReflectedPathLoss(txPos, rxPos, refPos, txFreq, matPerm), GetDelay(points, LIGHT_SPEED));
}
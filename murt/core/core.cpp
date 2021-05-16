#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include "structmember.h"
#include <numpy/arrayobject.h>

#include <vector>
#include <unordered_map>
#include <thread>

#include "record.hpp"
#include "vec3.hpp"
#include "bvh.hpp"
#include "triangle.hpp"
#include "tracer.hpp"
#include "calculator.hpp"

typedef struct
{
    PyObject_HEAD
        Tracer *tracer;
} RayTracerObject;

static PyObject *RecordsToPyRecords(std::vector<Record> records)
{
    PyObject *py_records = PyList_New(records.size());
    for (size_t i = 0; i < records.size(); ++i)
    {
        Record &record = records[i];
        PyObject *py_record = nullptr;
        if (record.type == RecordType::Direct)
        {
            py_record = Py_BuildValue("[i]", 1);
        }
        else if (record.type == RecordType::Diffracted)
        {
            size_t n = record.points.size();
            PyObject *diff_points = PyList_New(n);
            for (size_t j = 0; j < n; ++j)
            {
                Vec3 &dif_pos = record.points[j];
                PyObject *py_diff_pos = Py_BuildValue("(f f f)", dif_pos.x_, dif_pos.y_, dif_pos.z_);
                PyList_SetItem(diff_points, j, py_diff_pos);
            }
            py_record = Py_BuildValue("i O", 2, diff_points);
        }
        else if (record.type == RecordType::SingleReflected)
        {
            float ref_x = record.points[0].x_;
            float ref_y = record.points[0].y_;
            float ref_z = record.points[0].z_;

            py_record = Py_BuildValue("i (f f f)", 3, ref_x, ref_y, ref_z);
        }
        PyList_SetItem(py_records, i, py_record);
    }
    return py_records;
}

static PyObject *Trace(RayTracerObject *self, PyObject *args)
{
    PyArrayObject *txArrObj;
    PyArrayObject *rxArrObj;
    if (!PyArg_ParseTuple(args, "O|O", &txArrObj, &rxArrObj))
        return NULL;

    Vec3 txPos, rxPos;
    txPos.x_ = *(float *)PyArray_GETPTR1(txArrObj, 0);
    txPos.y_ = *(float *)PyArray_GETPTR1(txArrObj, 1);
    txPos.z_ = *(float *)PyArray_GETPTR1(txArrObj, 2);

    rxPos.x_ = *(float *)PyArray_GETPTR1(rxArrObj, 0);
    rxPos.y_ = *(float *)PyArray_GETPTR1(rxArrObj, 1);
    rxPos.z_ = *(float *)PyArray_GETPTR1(rxArrObj, 2);

    std::vector<Record> records = (self->tracer)->Trace(txPos, rxPos);

    PyObject *py_records = RecordsToPyRecords(records);

    return py_records;
}

static PyObject *HitNearest(RayTracerObject *self, PyObject *args)
{
    PyArrayObject *txArrObj;
    PyArrayObject *rxArrObj;
    if (!PyArg_ParseTuple(args, "O|O", &txArrObj, &rxArrObj))
        return NULL;

    Vec3 fromPos, toPos;
    fromPos.x_ = *(float *)PyArray_GETPTR1(txArrObj, 0);
    fromPos.y_ = *(float *)PyArray_GETPTR1(txArrObj, 1);
    fromPos.z_ = *(float *)PyArray_GETPTR1(txArrObj, 2);

    toPos.x_ = *(float *)PyArray_GETPTR1(rxArrObj, 0);
    toPos.y_ = *(float *)PyArray_GETPTR1(rxArrObj, 1);
    toPos.z_ = *(float *)PyArray_GETPTR1(rxArrObj, 2);

    return Py_BuildValue("f", (self->tracer)->HitNearest(fromPos, toPos));
}

static PyObject *IsOutdoor(RayTracerObject *self, PyObject *args)
{
    PyArrayObject *arrObj;
    if (!PyArg_ParseTuple(args, "O", &arrObj))
        return NULL;

    Vec3 position;
    position.x_ = *(float *)PyArray_GETPTR1(arrObj, 0);
    position.y_ = *(float *)PyArray_GETPTR1(arrObj, 1);
    position.z_ = *(float *)PyArray_GETPTR1(arrObj, 2);

    return Py_BuildValue("i", (self->tracer)->IsOutdoor(position));
}

static PyObject *GetID(RayTracerObject *self, PyObject *Py_UNUSED(ignored))
{
    return Py_BuildValue("I", (self->tracer)->id_);
}

static PyObject *GetTracedVolume(RayTracerObject *self, PyObject *args)
{
    float x_min, x_max;
    float y_min, y_max;
    float z_min, z_max;
    int x_n, y_n, z_n;
    PyArrayObject *txPosObj;
    float tx_freq, mat_perm;
    int max_thread = 0;
    if (!PyArg_ParseTuple(args, "f|f|i|f|f|i|f|f|i|O|f|f|i",
                          &x_min, &x_max, &x_n,
                          &y_min, &y_max, &y_n,
                          &z_min, &z_max, &z_n,
                          &txPosObj, &tx_freq, &mat_perm,
                          &max_thread))
        return NULL;

    Vec3 txPos;
    txPos.x_ = *(float *)PyArray_GETPTR1(txPosObj, 0);
    txPos.y_ = *(float *)PyArray_GETPTR1(txPosObj, 1);
    txPos.z_ = *(float *)PyArray_GETPTR1(txPosObj, 2);

    Tracer *tracer = self->tracer;

    float x_delta = (x_max - x_min) / (x_n - 1);
    float y_delta = (y_max - y_min) / (y_n - 1);
    float z_delta = (z_max - z_min) / (z_n - 1);

    std::vector<std::thread> thread_vectors;
    size_t threads_n;

    if (max_thread == 0)
    {
        threads_n = std::thread::hardware_concurrency() * 5;
    }
    else if (max_thread < 0)
    {
        threads_n = 65535;
    }
    else
    {
        threads_n = max_thread;
    }

    for (int x_itr = 0; x_itr < x_n; ++x_itr)
        for (int y_itr = 0; y_itr < y_n; ++y_itr)
            for (int z_itr = 0; z_itr < z_n; ++z_itr)
            {
                Vec3 rxPos;
                rxPos.x_ = x_min + x_delta * x_itr;
                rxPos.y_ = y_min + y_delta * y_itr;
                rxPos.z_ = z_min + z_delta * z_itr;
                std::thread current_thread(&Tracer::TraceAsync, tracer,
                                           txPos, rxPos, tx_freq, mat_perm);

                thread_vectors.push_back(std::move(current_thread));

                if (thread_vectors.size() >= threads_n)
                {
                    for (std::thread &thread : thread_vectors)
                        thread.join();
                    thread_vectors.clear();
                }
            }

    // join threads
    for (std::thread &thread : thread_vectors)
        thread.join();
    thread_vectors.clear();

    std::vector<std::vector<float> > &results = tracer->volume_result_;
    PyObject *list_results = PyList_New(results.size());
    for (size_t i = 0; i < results.size(); ++i)
    {
        std::vector<float> &result = results[i];
        PyObject *list_result = Py_BuildValue("f f f f",
                                              result[0], result[1],
                                              result[2], result[3]);

        PyList_SetItem(list_results, i, list_result);
    }
    //clean the resullts
    results.clear();
    return list_results;
}

static PyObject *RayTracerObjectNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    RayTracerObject *self;
    self = (RayTracerObject *)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        PyArrayObject *verticesArray;
        PyArrayObject *trianglesArray;

        if (!PyArg_ParseTuple(args, "O|O", &verticesArray, &trianglesArray))
            return NULL;

        size_t vert_dim_n = verticesArray->dimensions[0];
        size_t vert_dim_m = verticesArray->dimensions[1];
        size_t tri_dim_n = trianglesArray->dimensions[0];
        size_t tri_dim_m = trianglesArray->dimensions[1];
        if (vert_dim_m != 3 || tri_dim_m != 3)
            return NULL;

        std::unordered_map<size_t, Vec3> vertices_dict;
        for (size_t i = 0; i < vert_dim_n; ++i)
        {
            double x = *(double *)PyArray_GETPTR2(verticesArray, i, 0);
            double y = *(double *)PyArray_GETPTR2(verticesArray, i, 1);
            double z = *(double *)PyArray_GETPTR2(verticesArray, i, 2);
            //printf("Vertice #%lu => %.3f, %.3f, %.3f\n", i, x, y, z);
            vertices_dict[i] = Vec3((float)x, (float)y, (float)z);
        }
        // read to triangles
        std::vector<Triangle *> triangles;
        for (size_t i = 0; i < tri_dim_n; ++i)
        {
            size_t p1Idx = *(size_t *)PyArray_GETPTR2(trianglesArray, i, 0);
            size_t p2Idx = *(size_t *)PyArray_GETPTR2(trianglesArray, i, 1);
            size_t p3Idx = *(size_t *)PyArray_GETPTR2(trianglesArray, i, 2);
            //printf("Triangle #%lu => %lu, %lu, %lu\n", i, p1Idx, p2Idx, p3Idx);
            Vec3 p1Pos = vertices_dict[p1Idx];
            Vec3 p2Pos = vertices_dict[p2Idx];
            Vec3 p3Pos = vertices_dict[p3Idx];

            triangles.push_back(new Triangle(p1Pos, p2Pos, p3Pos));
        }
        //printf("triangles : %d\n", triangles.size());
        self->tracer = new Tracer(triangles);
    }
    return (PyObject *)self;
}

static int RayTracerObjectClear(RayTracerObject *self)
{
    if (self->tracer != nullptr)
    {
        delete self->tracer;
        self->tracer = nullptr;
    }
    return 0;
}

static void RayTracerObjectDealloc(RayTracerObject *self)
{
    RayTracerObjectClear(self);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

// static int RayTracerObjectInit(RayTracerObject *self, PyObject *args, PyObject *kwds)
// {
//     return 0;
// }
/*----------------------------------------------------------------*/

/*Define methods*/
static PyMethodDef RayTracerMethods[] = {
    {"trace", (PyCFunction)Trace, METH_VARARGS, "Trace rays from tx to rx"},
    {"isOutdoor", (PyCFunction)IsOutdoor, METH_VARARGS, "Check if the input position is indoor"},
    {"hitNearest", (PyCFunction)HitNearest, METH_VARARGS, "Check the nearest hit distance"},
    {"traceVolume", (PyCFunction)GetTracedVolume, METH_VARARGS, "Trace path loss volume"},
    {"getId", (PyCFunction)GetID, METH_NOARGS, "Return ID of Tracer"},
    {NULL},
};

static PyMemberDef RayTracerMembers[] = {
    {NULL}};

static PyTypeObject RayTracerType = {
    PyVarObject_HEAD_INIT(NULL, 0) "murtcore.Tracer", /* tp_name */
    sizeof(RayTracerObject),                          /* tp_basicsize */
    0,                                                /* tp_itemsize */
    (destructor)RayTracerObjectDealloc,               /* tp_dealloc */
    0,                                                /* tp_vectorcall_offset */
    0,                                                /* tp_getattr */
    0,                                                /* tp_setattr */
    0,                                                /* tp_as_async */
    0,                                                /* tp_repr */
    0,                                                /* tp_as_number */
    0,                                                /* tp_as_sequence */
    0,                                                /* tp_as_mapping */
    0,                                                /* tp_hash */
    0,                                                /* tp_call */
    0,                                                /* tp_str */
    0,                                                /* tp_getattro */
    0,                                                /* tp_setattro */
    0,                                                /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                               /* tp_flags */
    "Ray Tracer Objects",                             /* tp_doc */
    0,                                                /* tp_traverse */
    (inquiry)RayTracerObjectClear,                    /* tp_clear */
    0,                                                /* tp_richcompare */
    0,                                                /* tp_weaklistoffset */
    0,                                                /* tp_iter */
    0,                                                /* tp_iternext */
    RayTracerMethods,                                 /* tp_methods */
    RayTracerMembers,                                 /* tp_members */
    0,                                                /* tp_getset */
    0,                                                /* tp_base */
    0,                                                /* tp_dict */
    0,                                                /* tp_descr_get */
    0,                                                /* tp_descr_set */
    0,                                                /* tp_dictoffset */
    0,                                                /* tp_init */
    0,                                                /* tp_alloc */
    RayTracerObjectNew,                               /* tp_new */
};

static PyModuleDef murtcore_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "murtcore",
    .m_doc = "MURT Core Tracer", // TODO[]: To be documented.
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_core(void)
{
    if (PyType_Ready(&RayTracerType) < 0)
        return NULL;

    PyObject *module_;
    module_ = PyModule_Create(&murtcore_module);
    if (module_ == NULL)
        return NULL;

    Py_INCREF(&RayTracerType);
    if (PyModule_AddObject(module_, "Tracer", (PyObject *)&RayTracerType) < 0)
    {
        Py_DECREF(&RayTracerType);
        Py_DECREF(module_);
        return NULL;
    }
    return module_;
}
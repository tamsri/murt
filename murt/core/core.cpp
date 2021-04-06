#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include "structmember.h"
#include <numpy/arrayobject.h>

#include <vector>
#include <unordered_map>

#include "vec3.hpp"
#include "bvh.hpp"
#include "triangle.hpp"
#include "tracer.hpp"

typedef struct
{
    PyObject_HEAD
        Tracer *tracer;
} RayTracerObject;

static PyObject *Trace(RayTracerObject *self, PyObject *args)
{
    // TODO[]: Get args from numpy
    PyArrayObject *txArrObj;
    PyArrayObject *rxArrObj;
    if (!PyArg_ParseTuple(args, "O|O", &txArrObj, &rxArrObj))
        return NULL;

    // TODO[]: put to tracer
    Vec3 txPos, rxPos;
    txPos.x_ = (float)*(double *)PyArray_GETPTR2(txArrObj, 0, 0);
    txPos.y_ = (float)*(double *)PyArray_GETPTR2(txArrObj, 1, 0);
    txPos.z_ = (float)*(double *)PyArray_GETPTR2(txArrObj, 2, 0);

    rxPos.x_ = (float)*(double *)PyArray_GETPTR2(txArrObj, 0, 0);
    rxPos.y_ = (float)*(double *)PyArray_GETPTR2(txArrObj, 1, 0);
    rxPos.z_ = (float)*(double *)PyArray_GETPTR2(txArrObj, 2, 0);

    // TODO[]: trace
    (self->tracer)->Trace(txPos, rxPos);
    // TODO[]: construct object and return

    return Py_BuildValue("s", "yes");
}

static PyObject *GetID(RayTracerObject *self, PyObject *Py_UNUSED(ignored))
{
    return Py_BuildValue("I", (self->tracer)->id_);
}

static PyObject *RayTracerObjectNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    RayTracerObject *self;
    self = (RayTracerObject *)type->tp_alloc(type, 0);
    if (self != NULL)
    {

        // TODO[X]: Read args to triangles
        PyArrayObject *verticesArray;
        PyArrayObject *trianglesArray;

        if (!PyArg_ParseTuple(args, "O|O", &verticesArray, &trianglesArray))
            return NULL;

        size_t vert_dim_n = verticesArray->dimensions[0];
        size_t vert_dim_m = verticesArray->dimensions[1];
        size_t tri_dim_n = trianglesArray->dimensions[0];
        size_t tri_dim_m = trianglesArray->dimensions[1];
        //printf("vert: n=%lu, m=%lu\n", vert_dim_n, vert_dim_m);
        //printf("tria: n=%lu, m=%lu\n", tri_dim_n, tri_dim_m);
        if (vert_dim_m != 3 || tri_dim_m != 3)
            return NULL; // invalid input TODO[?]: maybe, bug it later.

        // TODO[]: Convert objects into vector<Triangle *> triangles
        std::unordered_map<size_t, Vec3> vertices_dict;
        // prepare vertices dictionary.
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
        // TODO[]: Send triangles to ray tracer
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

static int RayTracerObjectInit(RayTracerObject *self, PyObject *args, PyObject *kwds)
{
    return 0;
}
/*----------------------------------------------------------------*/

/*Define methods*/
static PyMethodDef RayTracerMethods[] = {
    {"trace", (PyCFunction)Trace, METH_VARARGS, "Trace rays from tx to rx"},
    {"getid", (PyCFunction)GetID, METH_NOARGS, "Return ID of Tracer"},
    {NULL}
    //{NULL, NULL, 0, NULL},
};

static PyMemberDef RayTracerMembers[] = {
    //{(char *)"id", T_UINT, offsetof(RayTracerObject, id), 0, (char *)"Tracer's ID"},
    {NULL}
    //{NULL, NULL, 0, NULL},
};

static PyTypeObject RayTracerType = {
    PyVarObject_HEAD_INIT(NULL, 0)    /* ob_size */
        .tp_name = "murtcore.Tracer", /*tp_name*/
    .tp_doc = "Ray Tracer Objects",
    .tp_basicsize = sizeof(RayTracerObject),          /*tp_basicsize*/
    .tp_dealloc = (destructor)RayTracerObjectDealloc, /*tp_dealloc*/
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_clear = (inquiry)RayTracerObjectClear,
    .tp_methods = RayTracerMethods,
    .tp_members = RayTracerMembers,
    .tp_init = 0, //(initproc)RayTracerObjectInit,
    .tp_new = RayTracerObjectNew,
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
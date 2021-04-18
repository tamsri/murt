#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "calculator.hpp"

static PyObject *GetDirectLoss(PyObject *self, PyObject *args)
{
    PyObject *txPosObj;
    PyObject *rxPosObj;
    float txFreq;
    if (!PyArg_ParseTuple(args, "O|O|f", &txPosObj, &rxPosObj, &txFreq))
        return NULL;

    Vec3 txPos;
    txPos.x_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 0));
    txPos.y_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 1));
    txPos.z_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 2));

    Vec3 rxPos;
    rxPos.x_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 0));
    rxPos.y_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 1));
    rxPos.z_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 2));

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
    if (!PyArg_ParseTuple(args, "O|O|O|f", &txPosObj, &rxPosObj, &edgesObj, &txFreq))
        return NULL;

    Vec3 txPos;
    txPos.x_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 0));
    txPos.y_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 1));
    txPos.z_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 2));

    Vec3 rxPos;
    rxPos.x_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 0));
    rxPos.y_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 1));
    rxPos.z_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 2));

    std::vector<Vec3> edges;
    for (int i = 0; i < PyList_Size(edgesObj); ++i)
    {
        PyObject *edgeValue = PyList_GetItem(edgesObj, i);
        Vec3 edge;
        edge.x_ = (float)PyFloat_AsDouble(PyList_GetItem(edgeValue, 0));
        edge.y_ = (float)PyFloat_AsDouble(PyList_GetItem(edgeValue, 1));
        edge.z_ = (float)PyFloat_AsDouble(PyList_GetItem(edgeValue, 2));
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
    if (!PyArg_ParseTuple(args, "O|O|O|f|f", &txPosObj, &rxPosObj, &refPosObj, &txFreq, &matPerm))
        return NULL;

    Vec3 txPos;
    txPos.x_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 0));
    txPos.y_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 1));
    txPos.z_ = (float)PyFloat_AsDouble(PyList_GetItem(txPosObj, 2));

    Vec3 rxPos;
    rxPos.x_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 0));
    rxPos.y_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 1));
    rxPos.z_ = (float)PyFloat_AsDouble(PyList_GetItem(rxPosObj, 2));

    Vec3 refPos;
    refPos.x_ = (float)PyFloat_AsDouble(PyList_GetItem(refPosObj, 0));
    refPos.y_ = (float)PyFloat_AsDouble(PyList_GetItem(refPosObj, 1));
    refPos.z_ = (float)PyFloat_AsDouble(PyList_GetItem(refPosObj, 2));

    std::vector<Vec3> points;
    points.push_back(txPos);
    points.push_back(refPos);
    points.push_back(rxPos);

    return Py_BuildValue("f f", ReflectedPathLoss(txPos, rxPos, refPos, txFreq, matPerm), GetDelay(points, LIGHT_SPEED));
}

static PyMethodDef CalculatorFunctions[] = {
    {"directLoss", (PyCFunction)GetDirectLoss, METH_VARARGS, "Calculate Free Space Path Loss"},
    {"reflectLoss", (PyCFunction)GetReflectionLoss, METH_VARARGS, "Calculate Reflection Loss"},
    {"diffractLoss", (PyCFunction)GetDiffractionLoss, METH_VARARGS, "Calculate Diffraction Loss"},
    {NULL}};

static PyModuleDef CalculatorModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "calculator",
    .m_doc = "The path loss calculator module",
    .m_size = -1,
    .m_methods = CalculatorFunctions,
};

PyMODINIT_FUNC PyInit_calculator(void)
{
    PyObject *m = PyModule_Create(&CalculatorModule);
    if (m == NULL)
        return NULL;

    return m;
}

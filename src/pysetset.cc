#include <Python.h>
#include "structmember.h"

#include "pysetset.h"

static void
setset_dealloc(PySetsetObject *so)
{
    so->ob_type->tp_free((PyObject *)so);
}

static PyObject *
setset_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PySetsetObject *so;

    so = (PySetsetObject *)type->tp_alloc(type, 0);

    return (PyObject *)so;
}

static int
setset_init(PySetsetObject* so, PyObject* args, PyObject* kwds)
{
    PyObject *s = NULL;
    if (! PyArg_ParseTuple(args, "|O", &s))
        return -1;
    if (s != NULL && ! (PySet_Check(s) || PyFrozenSet_Check(s))) {
        PyErr_SetString(PyExc_TypeError, "must be (frozen)set");
        return -1;
    }

    if (s == NULL)
        return 0;
/*
    PyObject* it = PyObject_GetIter(s);
    if (it == NULL)
        return -1;
    so->f = ZBDD(1);
    PyObject* elem;
    while ((elem = PyIter_Next(it))) {
        if (! PyInt_Check(elem)) {
            Py_DECREF(elem);
            PyErr_SetString(PyExc_TypeError, "must be (frozen)set of integers");
            return -1;
        }
        bddvar v = PyLong_AsLong(elem);
        if (v > max_var)
            for (; max_var < v; max_var++)
                ZBDD(1).Change(BDD_NewVarOfLev(1));
        Py_DECREF(elem);
        so->f *= ZBDD(1).Change(v);
    }
    Py_DECREF(it);
*/
    return 0;
}

static PyMemberDef setset_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef setset_methods[] = {
    {NULL}  /* Sentinel */
};

static PyObject *
setset_repr(PySetsetObject *so)
{
    return PyString_FromFormat("<%s object of %p>", so->ob_type->tp_name, (void *) so->ss.id());
}

static PyObject *
setset_sub(PySetsetObject *so, PyObject *other)
{
    PySetsetObject *result = NULL;

    if (!PySetset_Check(so) || !PySetset_Check(other)) {
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    }

    result = (PySetsetObject *)PySetset_Type.tp_alloc(&PySetset_Type, 0);
    if (result == NULL)
        return NULL;

    // TODO: add elements to result

    return (PyObject *)result;
}

// Returns the number of objects in sequence o on success, and -1 on failure.
static Py_ssize_t
setset_len(PyObject *self)
{
    return 0;
}

// If an item in o is equal to value, return 1, otherwise return 0. On error, return -1.
static int
setset_contains(PySetsetObject *so, PyObject *key)
{
    return 0;
}

static long
setset_hash(PyObject *self)
{
    PySetsetObject *so = (PySetsetObject *)self;
    return (long) so->ss.id();
}

static PyObject *
setset_richcompare(PySetsetObject *v, PyObject *w, int op)
{
    PySetsetObject *u;

    if(! PySetset_Check(w)) {
        if (op == Py_EQ)
            Py_RETURN_FALSE;
        if (op == Py_NE)
            Py_RETURN_TRUE;
        PyErr_SetString(PyExc_TypeError, "can only compare to set of sets");
        return NULL;
    }
    u = (PySetsetObject *)w;
    switch (op) {
    case Py_EQ:
        if (v->ss == u->ss) Py_RETURN_TRUE;
        else                    Py_RETURN_FALSE;
    case Py_NE:
        if (v->ss != u->ss) Py_RETURN_TRUE;
        else                    Py_RETURN_FALSE;
    default:
        PyErr_SetString(PyExc_TypeError, "not support enequalities");
        return NULL;
/*
    case Py_LE:
        if (setset_issubset(v, u)) Py_RETURN_TRUE;
        else                     Py_RETURN_FALSE;
    case Py_GE:
        if (setset_issuperset(v, u)) Py_RETURN_TRUE;
        else                       Py_RETURN_FALSE;
    case Py_LT:
        if (v->sets != u->sets && setset_issubset(v, u)) Py_RETURN_TRUE;
        else                                           Py_RETURN_FALSE;
    case Py_GT:
        if (v->sets != u->sets && setset_issuperset(v, u)) Py_RETURN_TRUE;
        else                                             Py_RETURN_FALSE;
*/
    }
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}

static PyNumberMethods setset_as_number = {
    0,                                  /*nb_add*/
    (binaryfunc)setset_sub,               /*nb_subtract*/
    0,                                  /*nb_multiply*/
    0,                                  /*nb_divide*/
    0,                                  /*nb_remainder*/
    0,                                  /*nb_divmod*/
    0,                                  /*nb_power*/
    0,                                  /*nb_negative*/
    0,                                  /*nb_positive*/
    0,                                  /*nb_absolute*/
    0,                                  /*nb_nonzero*/
    0,                                  /*nb_invert*/
    0,                                  /*nb_lshift*/
    0,                                  /*nb_rshift*/
//    (binaryfunc)setset_and,                /*nb_and*/
//    (binaryfunc)setset_xor,                /*nb_xor*/
//    (binaryfunc)setset_or,                 /*nb_or*/
    0,                                  /*nb_coerce*/
    0,                                  /*nb_int*/
    0,                                  /*nb_long*/
    0,                                  /*nb_float*/
    0,                                  /*nb_oct*/
    0,                                  /*nb_hex*/
    0,                                  /*nb_inplace_add*/
//    (binaryfunc)setset_isub,               /*nb_inplace_subtract*/
    0,                                  /*nb_inplace_multiply*/
    0,                                  /*nb_inplace_divide*/
    0,                                  /*nb_inplace_remainder*/
    0,                                  /*nb_inplace_power*/
    0,                                  /*nb_inplace_lshift*/
    0,                                  /*nb_inplace_rshift*/
//    (binaryfunc)setset_iand,               /*nb_inplace_and*/
//    (binaryfunc)setset_ixor,               /*nb_inplace_xor*/
//    (binaryfunc)setset_ior,                /*nb_inplace_or*/
};

static PySequenceMethods setset_as_sequence = {
    setset_len,                           /* sq_length */
    0,                                  /* sq_concat */
    0,                                  /* sq_repeat */
    0,                                  /* sq_item */
    0,                                  /* sq_slice */
    0,                                  /* sq_ass_item */
    0,                                  /* sq_ass_slice */
    (objobjproc)setset_contains,          /* sq_contains */
};

/***** Setset iterator type ***********************************************/

typedef struct {
    PyObject_HEAD
    PySetsetObject *si_sets; /* Set to NULL when iterator is exhausted */
    Py_ssize_t si_pos;
    Py_ssize_t len;
} setsetiterobject;

static void
setsetiter_dealloc(setsetiterobject *si)
{
    Py_XDECREF(si->si_sets);
    PyObject_Del(si);
}

static PyObject *
setsetiter_len(setsetiterobject *si)
{
    return PyInt_FromLong(0);
}

PyDoc_STRVAR(length_hint_doc, "Private method returning an estimate of len(list(it)).");

static PyMethodDef setsetiter_methods[] = {
    {"__length_hint__", (PyCFunction)setsetiter_len, METH_NOARGS, length_hint_doc},
    {NULL,              NULL}           /* sentinel */
};

static PyObject *setsetiter_iternext(setsetiterobject *si)
{
    PySetsetObject *so = si->si_sets;

    if (so == NULL)
        return NULL;
    assert(PySetset_Check(so));

    Py_DECREF(so);
    si->si_sets = NULL;
    return NULL;
}

static PyTypeObject SetsetIter_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "setsiterator",                             /* tp_name */
    sizeof(setsetiterobject),                     /* tp_basicsize */
    0,                                          /* tp_itemsize */
    /* methods */
    (destructor)setsetiter_dealloc,               /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_compare */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                         /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    PyObject_SelfIter,                          /* tp_iter */
    (iternextfunc)setsetiter_iternext,            /* tp_iternext */
    setsetiter_methods,                           /* tp_methods */
    0,
};

static PyObject *
setset_iter(PySetsetObject *so)
{
    setsetiterobject *si = PyObject_New(setsetiterobject, &SetsetIter_Type);
    if (si == NULL)
        return NULL;
    Py_INCREF(so);
    si->si_sets = so;
    si->si_pos = 0;
    si->len = 0;
    return (PyObject *)si;
}

PyTypeObject PySetset_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "_illion.setset",            /*tp_name*/
    sizeof(PySetsetObject),        /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)setset_dealloc,  /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    (reprfunc)setset_repr,       /*tp_repr*/
    &setset_as_number,           /*tp_as_number*/
    &setset_as_sequence,         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    setset_hash,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Base class for set of sets", /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    (richcmpfunc)setset_richcompare, /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    (getiterfunc)setset_iter,    /* tp_iter */
    0,		               /* tp_iternext */
    setset_methods,              /* tp_methods */
    setset_members,              /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)setset_init,       /* tp_init */
    PyType_GenericAlloc,       /* tp_alloc */
    setset_new,                  /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
init_illion(void) 
{
    PyObject* m;

    if (PyType_Ready(&PySetset_Type) < 0)
        return;

    m = Py_InitModule3("_illion", module_methods, "Hidden module to implement illion objects.");

    if (m == NULL)
      return;

    Py_INCREF(&PySetset_Type);
    PyModule_AddObject(m, "setset", (PyObject *)&PySetset_Type);
}

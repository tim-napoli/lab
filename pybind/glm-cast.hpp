/**
 * This file contains pybind11 casters for the glm library support.
 * In an ideal world, we should be able to cast C++ glm to PyGLM library,
 * but I don't know how to do this now.
 * Anyway, we cast glm types to python in an easy way:
 * - A vector of dimension n is a list of n float ;
 * - A matrix of dimension m is a list of m lists of m floats.
 */
#include <glm/glm.hpp>

namespace pybind11 { namespace detail {

// glm::mat -------------------------------------------------------------------

template <typename M, int D>
bool glm_mat_load(handle src, M& value) {
    for (size_t c = 0; c < D; c++) {
        PyObject* column = PyList_GetItem(src.ptr(), c);
        if (column == NULL) {
            return false;
        }
        for (size_t v = 0; v < D; v++) {
            PyObject* col_value = PyList_GetItem(column, v);
            if (col_value == NULL) {
                return false;
            }
            double dvalue = PyFloat_AsDouble(col_value);
            if (PyErr_Occurred() != NULL) {
                return false;
            }
            value[c][v] = dvalue;
            Py_DECREF(col_value);
        }
        Py_DECREF(column);
    }

    return true;
}

template <typename M, int D>
handle glm_mat_cast(M src) {
    PyObject* list = PyList_New(D);
    for (size_t c = 0; c < D; c++) {
        PyObject* column = PyList_New(D);
        for (size_t v = 0; v < D; v++) {
            PyList_SetItem(column, v, PyFloat_FromDouble(src[c][v]));
        }
        PyList_SetItem(list, c, column);
    }
    return list;
}

template <>
struct type_caster<glm::mat4> {
  public:
    PYBIND11_TYPE_CASTER(glm::mat4, _("glm_mat4"));

    bool load(handle src, bool) {
        return glm_mat_load<glm::mat4, 4>(src, value);
    }

    static handle cast(glm::mat4 src, return_value_policy policy,
                       handle parent)
    {
        return glm_mat_cast<glm::mat4, 4>(src);
    }
};

template <>
struct type_caster<glm::mat3> {
  public:
    PYBIND11_TYPE_CASTER(glm::mat3, _("glm_mat3"));

    bool load(handle src, bool) {
        return glm_mat_load<glm::mat3, 3>(src, value);
    }

    static handle cast(glm::mat3 src, return_value_policy policy,
                       handle parent)
    {
        return glm_mat_cast<glm::mat3, 3>(src);
    }
};

template <>
struct type_caster<glm::mat2> {
  public:
    PYBIND11_TYPE_CASTER(glm::mat2, _("glm_mat2"));

    bool load(handle src, bool) {
        return glm_mat_load<glm::mat2, 2>(src, value);
    }

    static handle cast(glm::mat2 src, return_value_policy policy,
                       handle parent)
    {
        return glm_mat_cast<glm::mat2, 2>(src);
    }
};

// glm::vec -------------------------------------------------------------------

template <typename M, int D>
bool glm_vec_load(handle src, M& value) {
    PyObject* vec = src.ptr();
    for (size_t i = 0; i < D; i++) {
        PyObject* vec_value = PyList_GetItem(vec, i);
        if (vec_value == NULL) {
            return false;
        }
        double dvalue = PyFloat_AsDouble(vec_value);
        if (PyErr_Occurred() != NULL) {
            return false;
        }
        value[i] = dvalue;
        Py_DECREF(vec_value);
    }

    return true;
}

template <typename M, int D>
handle glm_vec_cast(M src) {
    PyObject* list = PyList_New(D);
    for (size_t i = 0; i < D; i++) {
        PyList_SetItem(list, i, PyFloat_FromDouble(src[i]));
    }
    return list;
}

template <>
struct type_caster<glm::vec4> {
  public:
    PYBIND11_TYPE_CASTER(glm::vec4, _("glm_vec4"));

    bool load(handle src, bool) {
        return glm_vec_load<glm::vec4, 4>(src, value);
    }

    static handle cast(glm::vec4 src, return_value_policy policy,
                       handle parent)
    {
        return glm_vec_cast<glm::vec4, 4>(src);
    }
};

template <>
struct type_caster<glm::vec3> {
  public:
    PYBIND11_TYPE_CASTER(glm::vec3, _("glm_vec3"));

    bool load(handle src, bool) {
        return glm_vec_load<glm::vec3, 3>(src, value);
    }

    static handle cast(glm::vec3 src, return_value_policy policy,
                       handle parent)
    {
        return glm_vec_cast<glm::vec3, 3>(src);
    }
};

template <>
struct type_caster<glm::vec2> {
  public:
    PYBIND11_TYPE_CASTER(glm::vec2, _("glm_vec2"));

    bool load(handle src, bool) {
        return glm_vec_load<glm::vec2, 2>(src, value);
    }

    static handle cast(glm::vec2 src, return_value_policy policy,
                       handle parent)
    {
        return glm_vec_cast<glm::vec2, 2>(src);
    }
};

// ----------------------------------------------------------------------------

}}


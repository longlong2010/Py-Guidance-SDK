#include <Python.h>
#include "DJI_guidance.h"
#include "DJI_utility.h"

static PyObject* g_obj;

PyObject* wrap_reset_config(PyObject* self, PyObject* args) 
{
	int rc = reset_config();
	return Py_BuildValue("i", rc);
}

PyObject* wrap_init_transfer(PyObject* self, PyObject* args) 
{
	int err_code = init_transfer();
	return Py_BuildValue("i", err_code);
}

PyObject* wrap_start_transfer(PyObject* self, PyObject* args) 
{
	int err_code = start_transfer();
	return Py_BuildValue("i", err_code);
}

PyObject* wrap_stop_transfer(PyObject* self, PyObject* args) 
{
	int err_code = stop_transfer();
	return Py_BuildValue("i", err_code);
}

PyObject* wrap_release_transfer(PyObject* self, PyObject* args) 
{
	int err_code = release_transfer();
	return Py_BuildValue("i", err_code);
}

PyObject* wrap_select_imu(PyObject* self, PyObject* args) 
{
	select_imu();
	return Py_BuildValue("");
}

PyObject* wrap_select_ultrasonic(PyObject* self, PyObject* args) 
{
	select_ultrasonic();
	return Py_BuildValue("");
}

PyObject* wrap_select_obstacle_distance(PyObject* self, PyObject* args) 
{
	select_obstacle_distance();
	return Py_BuildValue("");
}

static void del_lock(PyObject *obj) {
    delete (DJI_lock*) PyCapsule_GetPointer(obj, "DJI_lock");
}

PyObject* wrap_create_lock(PyObject* self, PyObject* args) 
{
    DJI_lock* lock = new DJI_lock();
    return PyCapsule_New(lock, "DJI_lock", del_lock);
}

PyObject* wrap_enter(PyObject* self, PyObject* args) 
{
    DJI_lock* lock;
    PyObject* p_lock;
    if (!PyArg_ParseTuple(args,"O", &p_lock)) {
        return NULL;
    }
    if (!(lock = (DJI_lock*) PyCapsule_GetPointer(p_lock, "DJI_lock"))) {
        return NULL;
    }
    lock->enter();
	return Py_BuildValue("");
}

PyObject* wrap_leave(PyObject* self, PyObject* args) 
{
    DJI_lock* lock;
    PyObject* p_lock;
    if (!PyArg_ParseTuple(args,"O", &p_lock)) {
        return NULL;
    }
    if (!(lock = (DJI_lock*) PyCapsule_GetPointer(p_lock, "DJI_lock"))) {
        return NULL;
    }
    lock->leave();
	return Py_BuildValue("");
}

static void del_event(PyObject *obj) {
    delete (DJI_event*) PyCapsule_GetPointer(obj, "DJI_event");
}

PyObject* wrap_create_event(PyObject* self, PyObject* args) 
{
    DJI_event* event = new DJI_event();
    return PyCapsule_New(event, "DJI_event", del_event);
}

PyObject* wrap_wait_event(PyObject* self, PyObject* args) 
{
    DJI_event* event;
    PyObject* p_event;
    if (!PyArg_ParseTuple(args,"O", &p_event)) {
        return NULL;
    }
    if (!(event = (DJI_event*) PyCapsule_GetPointer(p_event, "DJI_event"))) {
        return NULL;
    }
    int rc = event->wait_event();
	return Py_BuildValue("i", rc);
}

PyObject* wrap_set_event(PyObject* self, PyObject* args) 
{
    DJI_event* event;
    PyObject* p_event;
    if (!PyArg_ParseTuple(args,"O", &p_event)) {
        return NULL;
    }
    if (!(event = (DJI_event*) PyCapsule_GetPointer(p_event, "DJI_event"))) {
        return NULL;
    }
    int rc = event->set_event();
	return Py_BuildValue("i", rc);
}

static int sdk_event_callback(int data_type, int data_len, char *content) {
    if (!PyCallable_Check(g_obj)) {
        return -1;
    }
    PyObject* args = Py_BuildValue("(is#)", data_type, content, data_len);
    PyObject* kwargs = NULL;
    PyObject_Call(g_obj, args, kwargs);
    Py_DECREF(args);
    Py_XDECREF(kwargs);
    if (PyErr_Occurred()) {
        PyErr_Print();
    }
    return 0;
}

PyObject* wrap_set_sdk_event_handler(PyObject* self, PyObject* args) 
{
    if (!PyArg_ParseTuple(args,"O", &g_obj)) {
        return NULL;
    }
    if (!PyCallable_Check(g_obj)) {
        return NULL;
    }
    int err_code = set_sdk_event_handler(sdk_event_callback);
	return Py_BuildValue("i", err_code);
}

static PyMethodDef guidanceMethods[] = 
{
	{"reset_config", wrap_reset_config, METH_VARARGS, "Clear subscribed configure"},
	{"init_transfer", wrap_init_transfer, METH_VARARGS, "Initialize Guidance and create data transfer thread"},
	{"start_transfer", wrap_start_transfer, METH_VARARGS, "Inform Guidance to start data transfer"},
	{"stop_transfer", wrap_stop_transfer, METH_VARARGS, "Inform Guidance to stop data transfer"},
	{"release_transfer", wrap_release_transfer, METH_VARARGS, "Release the data transfer thread"},
	{"select_imu", wrap_select_imu, METH_VARARGS, "Subscribe IMU data"},
	{"select_ultrasonic", wrap_select_ultrasonic, METH_VARARGS, "Subscribe ultrasonic data"},
	{"select_obstacle_distance", wrap_select_obstacle_distance, METH_VARARGS, "Subscribe obstacle distance"},
	{"create_lock", wrap_create_lock, METH_VARARGS, "Create lock"},
	{"enter", wrap_enter, METH_VARARGS, "Lock enter"},
	{"leave", wrap_leave, METH_VARARGS, "Lock leave"},
	{"create_event", wrap_create_event, METH_VARARGS, "Create event"},
	{"wait_event", wrap_wait_event, METH_VARARGS, "Wait event"},
	{"set_event", wrap_set_event, METH_VARARGS, "Set event"},
	{"set_sdk_event_handler", wrap_set_sdk_event_handler, METH_VARARGS, "Set callback function handler"},
	{NULL, NULL}
};

extern "C" void initguidance() {
	Py_InitModule("guidance", guidanceMethods);
}

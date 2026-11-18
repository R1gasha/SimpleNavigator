#include <pybind11/pybind11.h>
#include <utility>

#include "cpp_containers/s21_stack.h"   // сам подтянет s21_list.h
#include "cpp_containers/s21_queue.h"

namespace py = pybind11;

// Конкретные инстанциации шаблонов под «любой Python-объект»
using PyStack = s21::stack<py::object>;
using PyQueue = s21::queue<py::object>;

PYBIND11_MODULE(s21py, m) {
    m.doc() = "s21::stack / s21::queue exposed to Python";

    py::class_<PyStack>(m, "S21_Stack")
        .def(py::init<>())
        // push(const_reference value) -> void
        .def("push",  &PyStack::push)
        // pop() -> void, а top() -> const_reference
        .def("pop",   &PyStack::pop)
        .def("top",   &PyStack::top)      // вернёт копию py::object
        .def("empty", &PyStack::empty)
        .def("size",  &PyStack::size);

    py::class_<PyQueue>(m, "S21_Queue")
        .def(py::init<>())
        .def("push",  &PyQueue::push)
        .def("pop",   &PyQueue::pop)
        .def("front", &PyQueue::front)
        .def("back",  &PyQueue::back)
        .def("empty", &PyQueue::empty)
        .def("size",  &PyQueue::size);
}
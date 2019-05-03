//
// Created by KHML on 2019-05-02.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <vector>

#include "../src/Othello.hpp"


namespace py = pybind11;

class PyOthello : public Othello
{
public:
    py::array_t <int> getBoard()
    {
        py::array_t <int> pyBoard(boardSize *boardSize);
        int index = 0;
        for (int y = 0; y < boardSize; y++)
            for (int x = 0; x < boardSize; x++)
            {
                *pyBoard.mutable_data(index) = (int)board[getPos(x, y)];
                index++;
            }
        return pyBoard;
    }
};

PYBIND11_MODULE(othello, m)
{
    m.doc() = "Python Othello Module implemented by C++";

    py::enum_<Color>(m, "Color")
        .value("Wall", Wall)
        .value("Empty", Empty)
        .value("Black", Black)
        .value("White", White);

    py::class_<Move>(m, "Move")
        .def(py::init<int&, int&>())
        .def_readonly("x", &Move::x)
        .def_readonly("y", &Move::y);

    py::class_<PyOthello>(m,"Othello")
        .def(py::init())
        .def("play", &PyOthello::play, "put stone", py::arg("move"), py::arg("color"))
        .def("isLegal", &PyOthello::isLegal, "check move is legal or not", py::arg("move"), py::arg("color"))
        .def("board", &PyOthello::getBoard, "get board data as np.nd-array")
        .def("legal", &PyOthello::legal, "get legal moves for now situation.", py::arg("color"))
        .def("isFinished", &PyOthello::isFinished, "check game finished or not")
        .def("show", &PyOthello::show, "print board condition");
}


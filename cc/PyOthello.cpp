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
        .def("play", &PyOthello::play)
        .def("isLegal", &PyOthello::isLegal)
        .def("board", &PyOthello::getBoard)
        .def("legal", &PyOthello::legal)
        .def("isFinished", &PyOthello::isFinished)
        .def("show", &PyOthello::show);
}


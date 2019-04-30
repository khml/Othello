//
// Created by KHML
//

#ifndef OTHELLO_OTHELLO_HPP
#define OTHELLO_OTHELLO_HPP


#define PAD 1
#define DIRECTION_NUM 8

#include <vector>
#include <string>

/* direction
 * 1 2 3
 * 4 * 5
 * 6 7 8
 */
enum Direction
{
    LeftUpDirection = 0,
    UpDirection,
    RightUpDirection,
    LeftDirection,
    RightDirection,
    LeftDownDirection,
    DownDirection,
    RightDownDirection,
};

enum Color
{
    Wall = -1,
    Empty = 0,
    Black = 1,
    White = 2,
};

class Othello
{
public:
    Othello();

    Othello(const Othello &orig);

    virtual ~Othello();

    void play(const int x, const int y, const Color color);

    bool isLegal(const int x, const int y, const Color color);

    std::vector<int> legal(const Color color);

    bool isFinished();

    void show();

private:
    int const boardSize = 6;
    int const realBoardSize = boardSize + PAD * 2;
    std::vector<Color> board;
    int const eightNeighbour[DIRECTION_NUM] =
            {
                    /*LeftDirection UP*/ -1 * realBoardSize - 1,
                    /*UP*/ -1 * realBoardSize,
                    /*RightDirection UP*/ -1 * realBoardSize + 1,
                    /*LeftDirection*/ -1,
                    /*RightDirection*/ 1,
                    /*LeftDirection DownDirection*/ realBoardSize - 1,
                    /*DownDirection*/ realBoardSize,
                    /*RightDirection DownDirection*/ realBoardSize + 1
            };

    std::vector<int> neighbours;

    void initializeBoard();

    int getOppositeColor(const Color color);

    int getPos(const int x, const int y, const int space = PAD);

    void getXY(const int pos, int XY[2], const int space = PAD);

    int eightNeighbourDirection(const int pos, const Direction direction);

    bool isLegalPos(const int pos, const Color color);

    int seekColorForOneDirection(const int centerPos, const int direction, const Color color);

    void putStone(const int pos, const Color color);

    void assignNeighbours(const int pos);

    void deleteDuplicateOfNeighbours();

    void turnStones(const int pos, const Color color);

    bool isBoardFull();

    std::string toSymbol(const int color);


};


#endif //OTHELLO_OTHELLO_HPP

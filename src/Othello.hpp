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

Color getOppositeColor(Color color);


struct Move
{
public:
    Move(int x, int y) : x(x), y(y)
    {};
    const int x;
    const int y;
};

class Othello
{
public:
    Othello();

    Othello(const Othello &orig);

    virtual ~Othello();

    bool play(Move move, Color color);

    bool isLegal(Move move, Color color);

    std::vector<Move> legal(Color color);

    bool isFinished();

    void show();

protected:
    int boardSize = 8;
    int realBoardSize = boardSize + PAD * 2;
    std::vector<Color> board;
    const int eightNeighbour[DIRECTION_NUM] =
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

    int getPos(int x, int y, int space = PAD);

    int getPos(Move move, int space = PAD);

    Move getMove(int pos, int space = PAD);

    int eightNeighbourDirection(int pos, Direction direction, int repeat = 1);

    bool isLegalPos(int pos, Color color);

    bool isLegalRange(Move move, bool isReal = false);

    bool isLegalRange(int pos, bool isReal = false);

    int seekColorForOneDirection(int centerPos, Direction direction, Color color);

    void putStone(int pos, Color color);

    void assignNeighbours(int pos);

    void deleteDuplicateOfNeighbours();

    void turnStones(int pos, Color color);

    bool isBoardFull();
};

std::string toSymbol(int color);


#endif //OTHELLO_OTHELLO_HPP

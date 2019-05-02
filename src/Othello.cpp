//
// Created by KHML
//

#include <iostream>
#include <algorithm>

#include "Othello.hpp"

using std::cerr;
using std::endl;
using std::string;
using std::vector;

Color getOppositeColor(const Color color)
{
    return Color(3 - (int) color);
}

Othello::Othello()
{
    initializeBoard();
}

Othello::Othello(const Othello &orig) : boardSize(orig.boardSize), board(orig.board), realBoardSize(orig.realBoardSize)
{}

Othello::~Othello()
{}

void Othello::initializeBoard()
{
    // clear board and fill with EMPTY
    vector<Color>().swap(board);
    board.resize(realBoardSize * realBoardSize, Empty);

    // making Wall
    for (int i = 0; i < realBoardSize; i++)
    {
        board[getPos(0, i, 0)] = Wall;
        board[getPos(realBoardSize - 1, i, 0)] = Wall;
        board[getPos(i, 0, 0)] = Wall;
        board[getPos(i, realBoardSize - 1, 0)] = Wall;
    }

    /* deploy first stones;
     *   _ _ _ _
     *  |.|.|.|.|
     *  |.|X|O|.|
     *  |.|O|X|.|
     *  |.|.|.|.|
     *   ￣￣￣￣
     */
    int mid_pos = getPos(boardSize / 2 - 1, boardSize / 2 - 1);
    putStone(mid_pos, Black);
    putStone(eightNeighbourDirection(mid_pos, RightDownDirection), Black);
    putStone(eightNeighbourDirection(mid_pos, RightDirection), White);
    putStone(eightNeighbourDirection(mid_pos, DownDirection), White);
    deleteDuplicateOfNeighbours();
}

int inline Othello::eightNeighbourDirection(int pos, const Direction direction, const int repeat)
{
    for (int i = 0; i < repeat; i++)
    {
        pos += eightNeighbour[(int) direction];
    }
    return pos;
}

int Othello::getPos(const int x, const int y, const int space)
{
    return (x + space) + realBoardSize * (y + space);
}

int Othello::getPos(const Move move, const int space)
{
    return getPos(move.x, move.y, space);
}

Move Othello::getMove(const int pos, const int space)
{
    return {pos % realBoardSize - space, pos / realBoardSize - space};
}

/*
 * seek specified color in one direction.
 * return pos of first found one when found.
 * return negative value when not found.
 * ignore centerPos.
 */
int Othello::seekColorForOneDirection(const int centerPos, const Direction direction, const Color colorLookingFor)
{
    int seekPos = eightNeighbourDirection(centerPos, direction);
    Color oppositeColor = getOppositeColor(colorLookingFor);
    for (int i = 0; i < boardSize; i++)
    {
        Color seekColor = board[seekPos];
        if (seekColor == colorLookingFor)
        {
            return seekPos;
        }

        if (seekColor == oppositeColor)
        {
            seekPos = eightNeighbourDirection(seekPos, direction);
            continue;
        } else
            break;

    }
    return -1;
}

void Othello::turnStones(const int pos, const Color color)
{
    int const oppositeColor = getOppositeColor(color);
    int foundPos;
    int turnPos;

    for (int directionInt = 0; directionInt < DIRECTION_NUM; directionInt++)
    {
        auto direction = Direction(directionInt);
        // seek turnable direction
        if (board[eightNeighbourDirection(pos, direction)] != oppositeColor)
            continue;
        // seek same color pos
        foundPos = seekColorForOneDirection(pos, direction, color);
        if (foundPos > 0)
        {
            // init turnStones position
            turnPos = eightNeighbourDirection(pos, direction);
            for (int i = 0; i < boardSize; i++)
            {
                putStone(turnPos, color);
                turnPos = eightNeighbourDirection(turnPos, direction);
                if (turnPos == foundPos)
                    break;
            }
        }
    }
}

bool Othello::isLegalPos(const int pos, const Color color)
{
    if (board[pos] != Empty)
        return false;

    int const opposite_color = getOppositeColor(color);
    Direction direction;
    int seekPos;
    for (int i = 0; i < DIRECTION_NUM; i++)
    {
        direction = Direction(i);

        if (board[eightNeighbourDirection(pos, direction)] != opposite_color)
            continue;

        seekPos = eightNeighbourDirection(pos, direction); /*repeat=2, that's why 0 is self, 1 is just a next*/
        if (!isLegalRange(getMove(seekPos)))
            continue;

        if (seekColorForOneDirection(seekPos, direction, color) > 0)
            return true;

    }
    return false;
}

bool Othello::isLegalRange(const Move move, bool isReal)
{
    int size;
    if (isReal)
        size = realBoardSize;
    else
        size = boardSize;

    return !(move.x < 0 || size < move.x || move.y < 0 || size < move.y);
}

bool Othello::isLegalRange(const int pos, bool isReal)
{
    return isLegalRange(getMove(pos), isReal);
}

bool Othello::isLegal(const Move move, const Color color)
{
    if (!isLegalRange(move))
        return false;

    return isLegalPos(getPos(move), color);
}

vector<Move> Othello::legal(const Color color)
{
    vector<Move> legalMoves;
    for (auto pos : neighbours)
    {
        if (isLegalPos(pos, color))
            legalMoves.push_back(getMove(pos));
    }
    return legalMoves;
}

bool Othello::isBoardFull()
{
    int y, x = 0;

    for (y = 0; y < boardSize; y++)
    {
        for (x = 0; x < boardSize; x++)
        {
            if (board[getPos(x, y)] == Empty)
            {
                return false;
            }
        }
    }
    return true;
}

bool Othello::isFinished()
{
    if (isBoardFull())
        return true;

    if (neighbours.empty())
        return true;

    if (legal(Black).empty() && legal(White).empty())
        return true;

    return false;
}

void Othello::putStone(const int pos, const Color color)
{
    board[pos] = color;
    assignNeighbours(pos);
}

void Othello::assignNeighbours(const int pos)
{
    if (!neighbours.empty())
    {
        auto positionOfPos = std::find(neighbours.begin(), neighbours.end(), pos);
        if (positionOfPos != neighbours.end())
            neighbours.erase(positionOfPos);
    }

    for (int directionInt = 0; directionInt < DIRECTION_NUM; directionInt++)
    {
        int neighbour = eightNeighbourDirection(pos, Direction(directionInt));
        if (!isLegalRange(neighbour))
            continue;
        if (board[neighbour] == Empty)
            neighbours.push_back(neighbour);
    }
}

void Othello::deleteDuplicateOfNeighbours()
{
    std::sort(neighbours.begin(), neighbours.end());
    auto startOfUnused = std::unique(neighbours.begin(), neighbours.end());
    neighbours.erase(startOfUnused, neighbours.end());
}

bool Othello::play(const Move move, const Color color)
{
    if (!isLegal(move, color))
        return false;

    int pos = getPos(move);
    putStone(pos, color);
    turnStones(pos, color);
    deleteDuplicateOfNeighbours();
    return true;
}

string toSymbol(const int color)
{
    if (color == Black)
        return "B";
    else if (color == White)
        return "W";
    else if (color == Wall)
        return "#";
    else if (color == Empty)
        return " ";
    else
        return "*";
}

void Othello::show()
{
    for (int y = 0; y < realBoardSize; y++)
    {
        auto stones = string("|");
        for (int x = 0; x < realBoardSize; x++)
        {
            int pos = getPos(x, y, 0);
            Move move = getMove(pos, 0);
            if (!isLegalRange(move, true))
                return;
            stones += toSymbol(board[pos]) + "| ";
        }
        cerr << stones << endl;
    }
    cerr << endl;
}

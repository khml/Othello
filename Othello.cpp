//
// Created by KHML
//

#include <iostream>
#include <algorithm>

#include "Othello.hpp"


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
    std::vector<Color>().swap(board);
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

    std::cout << "initilize Board OK" << std::endl;
    show();
}

int Othello::getOppositeColor(const Color color)
{
    return Color(3 - (int) color);
}

int inline Othello::eightNeighbourDirection(const int pos, const Direction direction)
{
    return pos + eightNeighbour[(int) direction];
}

int Othello::getPos(const int x, const int y, const int space)
{
    return (x + space) + realBoardSize * (y + space);
}

int Othello::getPos(const Move move, const int space)
{
    return getPos(move.x, move.y);
}

Move Othello::getMove(const int pos, const int space)
{
    return Move(pos % realBoardSize - space, pos / realBoardSize - space);
}

/*
 * from pos, to one direction, seek color
 * return found pos
 * (color of pos is ignored)
 */
int Othello::seekColorForOneDirection(const int centerPos, const int direction, const Color colorLookingFor)
{
    int flag = -1;
    int seekPos = centerPos;
    for (int i = 0; i < realBoardSize; i++)
    {
        seekPos += eightNeighbour[direction];
        if (board[seekPos] == colorLookingFor)
        {
            flag = seekPos;
            break;
        }

        if (board[seekPos] == Wall || board[seekPos] == Empty)
            break;
    }
    return flag;
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
    bool neighbourFlag = false;

    if (board[pos] != Empty)
        return neighbourFlag;

    int const opposite_color = getOppositeColor(color);
    // explorer 8 neighbour
    for (int i = 0; i < DIRECTION_NUM; i++)
    {
        auto direction = Direction(i);
        if (board[eightNeighbourDirection(pos, direction)] == opposite_color &&
            seekColorForOneDirection(pos, direction, color))
        {
            neighbourFlag = true;
            break;
        }
    }
    return neighbourFlag;
}

bool Othello::isLegal(const Move move, const Color color)
{
    if (move.x < 0 || boardSize < move.x || move.y < 0 || boardSize < move.y)
        return false;

    return isLegalPos(getPos(move), color);
}

std::vector<Move> Othello::legal(const Color color)
{
    std::vector<Move> legalMoves;
    for (auto pos = neighbours.begin(); pos != neighbours.end(); ++pos)
    {
        if (isLegalPos(*pos, color))
            legalMoves.push_back(getMove(*pos));
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
            if (board[getPos(y, x)] == Empty)
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

    if (neighbours.size() == 0)
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

void Othello::play(const Move move, const Color color)
{
    int pos = getPos(move);
    if (isLegal(move, color))
    {
        putStone(pos, color);
        turnStones(pos, color);
        deleteDuplicateOfNeighbours();
    } else
    {
        std::cout << "Error Ilegal Position: x = " << move.x << ", y = " << move.y << std::endl;
        Color origColor = board[getPos(move)];
        putStone(getPos(move), color);
        show();
        putStone(getPos(move), origColor);

    }

    show();
}

std::string Othello::toSymbol(const int color)
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
        auto stones = std::string("|");
        for (int x = 0; x < realBoardSize; x++)
        {
            stones += toSymbol(board[getPos(x, y, 0)]) + "| ";
        }
        std::cout << stones << std::endl;
    }
    std::cout << std::endl;
}

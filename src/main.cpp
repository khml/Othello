//
// Created by KHML
//

#include <iostream>
#include "Othello.hpp"

void playWithCommandLine()
{
    Othello game;
    while (!game.isFinished())
    {
        std::string command;
        std::cout << "input as x,y,color = ";
        std::cin >> command;
        auto color = Color(std::stoi(command.substr(4, 1)));
        auto move = Move(std::stoi(command.substr(0, 1)), std::stoi(command.substr(2, 1)));
        game.play(move, color);
        game.show();
    }
}

void autoPlay()
{
    Othello game;
    game.show();
    Color color = Black;
    while (!game.isFinished())
    {
        auto candidates = game.legal(color);
        if (!candidates.empty())
        {
            auto move = candidates[0];
            game.play(move, color);
        }
        color = getOppositeColor(color);
        game.show();
    }
}

void playOthello()
{
    autoPlay();
}


/*
 *
 */
int main(int argc, char **argv)
{
    playOthello();
    return 0;
}

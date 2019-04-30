//
// Created by KHML
//

#include <iostream>
#include "Othello.hpp"

void playWithCommandLine(Othello &game)
{
    std::string command;
    std::cout << "input as x,y,color = ";
    std::cin >> command;
    auto color = Color(std::stoi(command.substr(4, 1)));
    game.play(std::stoi(command.substr(0, 1)), std::stoi(command.substr(2, 1)), color);
}

void playOthello()
{
    Othello game;
    while (!game.isFinished())
    {
        playWithCommandLine(game);
    }
}


/*
 *
 */
int main(int argc, char **argv)
{
    playOthello();
    return 0;
}

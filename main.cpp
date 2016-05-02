#include <iostream>
#include <memory>
#include <Game/MainGame.h>

using  std::make_shared;
using  ForlornMelody::Game::MainGame;

int main()
{
    auto pGame = std::make_shared<MainGame>();
    pGame->run();

    return 0;
}
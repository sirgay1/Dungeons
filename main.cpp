#include "game.h"
#include "DefaultAlgorithm.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string filename = "in.txt";
    if (argc > 1) filename = argv[1];

    Game game;
    if (!game.loadFromFile(filename)) return 1;

    DefaultAlgorithm defaultAlg;
    game.setAlgorithm(&defaultAlg);

    game.run();   // после выполнения в result.txt будет полный лог и финальный результат
    return 0;
}
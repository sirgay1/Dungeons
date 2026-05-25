#include "game.h"
#include "parser.h"
#include "DefaultAlgorithm.h"
#include "OutputRedirector.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

Game::Game() : currentRoomId(0), targetResource(nullptr), isRunning(true), algorithm(nullptr) {}

Room* Game::findRoom(int id) {
    for (auto& r : rooms) if (r.getId() == id) return &r;
    return nullptr;
}
const Room* Game::findRoom(int id) const {
    for (const auto& r : rooms) if (r.getId() == id) return &r;
    return nullptr;
}

bool Game::loadFromFile(const std::string& filename) {
    try {
        ParsedData data = parseInputFile(filename);
        rooms = std::move(data.rooms);
        targetResource = data.targetResource;
        currentRoomId = 0;
        person.setFood(data.food);
        if (targetResource) person.setTargetResource(*targetResource);
        return true;
    }
    catch (const std::exception& e) {
        std::ofstream errOut("result.txt");
        if (errOut.is_open()) errOut << e.what() << "\n";
        return false;
    }
}

void Game::run() {
    if (rooms.empty()) {
        std::cerr << "Игра не загружена\n";
        return;
    }
    try {
        OutputRedirector redirector("result.txt");
        if (algorithm == nullptr) {
            static DefaultAlgorithm defaultAlg;
            algorithm = &defaultAlg;
        }
        algorithm->run(*this);
        person.printResult(std::cout);
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка во время выполнения: " << e.what() << std::endl;
    }
}
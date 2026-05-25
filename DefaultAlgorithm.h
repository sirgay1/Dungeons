#pragma once
#include "IAlgorithm.h"
#include "game.h"
#include "room.h"
#include "resource.h"
#include <unordered_set>
#include <vector>

// Реализация исходного алгоритма
class DefaultAlgorithm : public IAlgorithm {
public:
    DefaultAlgorithm() = default;
    ~DefaultAlgorithm() override = default;

    void run(Game& game) override;

private:
    std::unordered_set<int> visitedRooms;   // номера посещённых комнат

    int getResourceValueWithTarget(const Game& game, const Resource& res) const noexcept;
    const Resource* getBestResourceInRoom(const Game& game, const Room& room) const noexcept;
    void collectBestResourceInRoom(Game& game, Room& room); // выводит collect и state после сбора
    int findNextStepToClosestUnvisited(Game& game, int start);
    std::vector<int> bfsShortestPathToStart(Game& game, int start, const std::unordered_set<int>& visited);
    void autoExplore(Game& game);
    void autoReturn(Game& game);
};
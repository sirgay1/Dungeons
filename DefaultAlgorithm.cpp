#include "DefaultAlgorithm.h"
#include "person.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <string>
#include <array>

int DefaultAlgorithm::getResourceValueWithTarget(const Game& game, const Resource& res) const noexcept {
    int val = res.getValue();
    if (&res == game.getTargetResource()) val *= 2;
    return val;
}

const Resource* DefaultAlgorithm::getBestResourceInRoom(const Game& game, const Room& room) const noexcept {
    const std::array<const Resource*, 4> resources = { &Iron::instance(), &Gold::instance(), &Gems::instance(), &Exp::instance() };
    const Resource* bestRes = nullptr;
    int bestVal = -1;
    for (const auto* res : resources) {
        int amount = 0;
        if (res == &Iron::instance()) amount = room.getIron();
        else if (res == &Gold::instance()) amount = room.getGold();
        else if (res == &Gems::instance()) amount = room.getGems();
        else amount = room.getExp();
        if (amount > 0) {
            int val = getResourceValueWithTarget(game, *res);
            if (val > bestVal) { bestVal = val; bestRes = res; }
        }
    }
    return bestRes;
}

void DefaultAlgorithm::collectBestResourceInRoom(Game& game, Room& room) {
    const Resource* bestRes = getBestResourceInRoom(game, room);
    if (bestRes) {
        std::string name;
        if (bestRes == &Iron::instance()) name = "iron";
        else if (bestRes == &Gold::instance()) name = "gold";
        else if (bestRes == &Gems::instance()) name = "gems";
        else name = "exp";
        std::cout << "collect " << name << "\n";
        int collected = room.collect(*bestRes);
        game.getPerson().addResource(*bestRes, collected);
        room.printStatus(std::cout);
    }
}

int DefaultAlgorithm::findNextStepToClosestUnvisited(Game& game, int start) {
    std::unordered_map<int, int> dist, parent;
    std::queue<int> q;
    dist[start] = 0;
    q.push(start);
    int bestDist = -1, bestRoom = -1;
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (visitedRooms.find(cur) == visitedRooms.end()) {
            if (bestDist == -1 || dist[cur] < bestDist || (dist[cur] == bestDist && cur < bestRoom)) {
                bestDist = dist[cur];
                bestRoom = cur;
            }
        }
        const Room* curRoom = game.findRoom(cur);
        if (!curRoom) continue;
        for (int nb : curRoom->getNeighbors()) {
            if (dist.find(nb) == dist.end()) {
                dist[nb] = dist[cur] + 1;
                parent[nb] = cur;
                q.push(nb);
            }
        }
    }
    if (bestRoom == -1) return -1;
    int cur = bestRoom;
    while (parent[cur] != start) { cur = parent[cur]; if (cur == start) break; }
    return cur;
}

std::vector<int> DefaultAlgorithm::bfsShortestPathToStart(Game& game, int start, const std::unordered_set<int>& visited) {
    std::unordered_map<int, int> parent;
    std::queue<int> q;
    q.push(start);
    parent[start] = -1;
    bool found = false;
    while (!q.empty() && !found) {
        int cur = q.front(); q.pop();
        if (cur == 0) { found = true; break; }
        const Room* curRoom = game.findRoom(cur);
        if (!curRoom) continue;
        std::vector<int> nbrs = curRoom->getNeighbors();
        std::sort(nbrs.begin(), nbrs.end());
        for (int nb : nbrs) {
            if (visited.find(nb) != visited.end() && parent.find(nb) == parent.end()) {
                parent[nb] = cur;
                q.push(nb);
            }
        }
    }
    if (parent.find(0) == parent.end()) return {};
    std::vector<int> path;
    int cur = 0;
    while (cur != start) {
        path.push_back(cur);
        cur = parent[cur];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void DefaultAlgorithm::autoExplore(Game& game) {
    visitedRooms.clear();
    visitedRooms.insert(0);
    Room* startRoom = game.findRoom(0);
    if (startRoom) collectBestResourceInRoom(game, *startRoom);

    int totalFood = game.getPerson().getFood();
    int halfFood = totalFood / 2;
    int usedFood = 0;

    while (usedFood < halfFood) {
        const Room* curRoom = game.findRoom(game.getCurrentRoomId());
        if (!curRoom) break;
        std::vector<int> neighbors = curRoom->getNeighbors();
        std::sort(neighbors.begin(), neighbors.end());
        int nextRoom = -1;
        for (int nb : neighbors) {
            if (visitedRooms.find(nb) == visitedRooms.end()) { nextRoom = nb; break; }
        }
        if (nextRoom != -1) {
            std::cout << "go " << nextRoom << "\n";
            game.getPerson().consumeFood();
            usedFood++;
            game.setCurrentRoomId(nextRoom);
            visitedRooms.insert(nextRoom);
            Room* newRoom = game.findRoom(nextRoom);
            if (newRoom) {
                newRoom->printStatus(std::cout);
                collectBestResourceInRoom(game, *newRoom);
            }
        }
        else {
            int step = findNextStepToClosestUnvisited(game, game.getCurrentRoomId());
            if (step == -1) break;
            std::cout << "go " << step << "\n";
            game.getPerson().consumeFood();
            usedFood++;
            game.setCurrentRoomId(step);
            visitedRooms.insert(step);
            Room* newRoom = game.findRoom(step);
            if (newRoom) {
                newRoom->printStatus(std::cout);
                collectBestResourceInRoom(game, *newRoom);
            }
        }
    }
}

void DefaultAlgorithm::autoReturn(Game& game) {
    std::vector<int> path = bfsShortestPathToStart(game, game.getCurrentRoomId(), visitedRooms);
    if (path.empty()) return;
    for (size_t i = 0; i < path.size(); ++i) {
        if (!game.getPerson().hasFood()) break;
        std::cout << "go " << path[i] << "\n";
        game.getPerson().consumeFood();
        game.setCurrentRoomId(path[i]);
        if (path[i] != 0) {
            Room* room = game.findRoom(path[i]);
            if (room) room->printStatus(std::cout);
        }
    }
    while (game.getPerson().hasFood()) {
        std::vector<int> targetRooms = { 0 };
        for (int r : path) if (r != 0) targetRooms.push_back(r);
        std::sort(targetRooms.begin(), targetRooms.end());
        targetRooms.erase(std::unique(targetRooms.begin(), targetRooms.end()), targetRooms.end());

        bool anyResource = false;
        for (int rid : targetRooms) {
            Room* room = game.findRoom(rid);
            if (!room) continue;
            if (room->getIron() > 0 || room->getGold() > 0 || room->getGems() > 0 || room->getExp() > 0) {
                anyResource = true;
                room->printStatus(std::cout);
                const Resource* bestRes = getBestResourceInRoom(game, *room);
                if (bestRes) {
                    std::string name;
                    if (bestRes == &Iron::instance()) name = "iron";
                    else if (bestRes == &Gold::instance()) name = "gold";
                    else if (bestRes == &Gems::instance()) name = "gems";
                    else name = "exp";
                    std::cout << "collect " << name << "\n";
                    int collected = room->collect(*bestRes);
                    game.getPerson().addResource(*bestRes, collected);
                    game.getPerson().consumeFood();
                    room->printStatus(std::cout);
                    if (!game.getPerson().hasFood()) break;
                }
            }
        }
        if (!anyResource) break;
    }
}

void DefaultAlgorithm::run(Game& game) {
    autoExplore(game);
    autoReturn(game);
}
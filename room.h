#pragma once
#include <vector>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include "resource.h"

// Комната подземелья
class Room {
private:
    int id;                                          // номер комнаты
    std::vector<int> neighbors;                     // номера смежных комнат
    std::unordered_map<const Resource*, int> resources; // текущее количество ресурсов
    std::unordered_set<const Resource*> collectedFlags; // ресурсы, которые были собраны (для вывода '_')

public:
    Room(int id, const std::vector<int>& neighbors, int iron, int gold, int gems, int exp);
    ~Room() = default;

    int getId() const noexcept;
    const std::vector<int>& getNeighbors() const noexcept;

    int getIron() const noexcept;
    int getGold() const noexcept;
    int getGems() const noexcept;
    int getExp() const noexcept;

    // Собрать ресурс – обнулить его в комнате и вернуть собранное количество (может бросить исключение при поиске, но не noexcept)
    int collect(const Resource& resource);

    // Вывести статус комнаты: 0 -> '_' если ресурс был собран
    void printStatus(std::ostream& out) const;
};
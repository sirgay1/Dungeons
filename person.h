#pragma once
#include <ostream>
#include <unordered_map>
#include "resource.h"

// Игрок – хранит собранные ресурсы, текущую еду и целевой ресурс
class Person {
private:
    std::unordered_map<const Resource*, int> collected; // накопленные ресурсы
    const Resource* targetResource;                     // ресурс с удвоенной ценностью
    int food;                                           // оставшаяся еда

public:
    Person();
    ~Person() = default;

    void setTargetResource(const Resource& res) noexcept;
    void setFood(int amount) noexcept;
    bool hasFood() const noexcept;
    void consumeFood();                                 // бросает исключение, если еды нет
    int getFood() const noexcept;

    void addResource(const Resource& resource, int amount) noexcept;
    int getTotalValue() const noexcept;                 // суммарная ценность (удвоение целевого)
    void printResult(std::ostream& out) const;          // выводит result и totalValue
};
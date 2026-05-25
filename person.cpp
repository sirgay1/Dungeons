#include "person.h"
#include <stdexcept>

Person::Person() : targetResource(nullptr), food(0) {
    collected[&Iron::instance()] = 0;
    collected[&Gold::instance()] = 0;
    collected[&Gems::instance()] = 0;
    collected[&Exp::instance()] = 0;
}

void Person::setTargetResource(const Resource& res) noexcept { targetResource = &res; }
void Person::setFood(int amount) noexcept { food = amount; }
bool Person::hasFood() const noexcept { return food > 0; }
int Person::getFood() const noexcept { return food; }

void Person::consumeFood() {
    if (food <= 0) throw std::runtime_error("Недостаточно еды");
    --food;
}

void Person::addResource(const Resource& resource, int amount) noexcept {
    collected[&resource] += amount;
}

int Person::getTotalValue() const noexcept {
    int total = 0;
    for (const auto& pair : collected) {
        int value = pair.first->getValue();
        if (targetResource && pair.first == targetResource) value *= 2;
        total += pair.second * value;
    }
    return total;
}

void Person::printResult(std::ostream& out) const {
    out << "result "
        << collected.at(&Iron::instance()) << " "
        << collected.at(&Gold::instance()) << " "
        << collected.at(&Gems::instance()) << " "
        << collected.at(&Exp::instance()) << " "
        << getTotalValue() << "\n";
}
#include "room.h"

Room::Room(int id, const std::vector<int>& neighbors, int iron, int gold, int gems, int exp)
    : id(id), neighbors(neighbors) {
    resources[&Iron::instance()] = iron;
    resources[&Gold::instance()] = gold;
    resources[&Gems::instance()] = gems;
    resources[&Exp::instance()] = exp;
}

int Room::getId() const noexcept { return id; }
const std::vector<int>& Room::getNeighbors() const noexcept { return neighbors; }
int Room::getIron() const noexcept { return resources.at(&Iron::instance()); }
int Room::getGold() const noexcept { return resources.at(&Gold::instance()); }
int Room::getGems() const noexcept { return resources.at(&Gems::instance()); }
int Room::getExp() const noexcept { return resources.at(&Exp::instance()); }

int Room::collect(const Resource& resource) {
    auto it = resources.find(&resource);
    if (it == resources.end()) return 0;
    int amount = it->second;
    if (amount > 0) {
        it->second = 0;
        collectedFlags.insert(&resource);
    }
    return amount;
}

void Room::printStatus(std::ostream& out) const {
    out << "state " << id << " ";
    auto print = [&](const Resource& res) {
        int val = resources.at(&res);
        bool wasCollected = collectedFlags.find(&res) != collectedFlags.end();
        if (val == 0 && wasCollected) out << "_ ";
        else out << val << " ";
        };
    print(Iron::instance());
    print(Gold::instance());
    print(Gems::instance());
    print(Exp::instance());
    out << "\n";
}
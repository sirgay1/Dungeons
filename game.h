#pragma once
#include <string>
#include <vector>
#include "room.h"
#include "person.h"
#include "resource.h"
#include "IAlgorithm.h"

// √лавный класс игры Ц управл€ет загрузкой, запуском алгоритма и выводом результата
class Game {
public:
    Game();
    ~Game() = default;

    bool loadFromFile(const std::string& filename);
    void run();   // запускает алгоритм и записывает результат в result.txt

    // ћетоды, необходимые алгоритму (public)
    const std::vector<Room>& getRooms() const noexcept { return rooms; }
    Person& getPerson() noexcept { return person; }
    const Person& getPerson() const noexcept { return person; }
    int getCurrentRoomId() const noexcept { return currentRoomId; }
    void setCurrentRoomId(int id) noexcept { currentRoomId = id; }
    const Resource* getTargetResource() const noexcept { return targetResource; }
    Room* findRoom(int id);
    const Room* findRoom(int id) const;
    void setAlgorithm(IAlgorithm* alg) noexcept { algorithm = alg; }

private:
    std::vector<Room> rooms;
    Person person;
    int currentRoomId;
    const Resource* targetResource;
    bool isRunning;
    IAlgorithm* algorithm;   // используемый алгоритм (может быть заменЄн)
};
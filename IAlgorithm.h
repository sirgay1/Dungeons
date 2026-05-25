#pragma once

class Game;

// Интерфейс для алгоритма прохождения подземелья
class IAlgorithm {
public:
    virtual ~IAlgorithm() = default;    // виртуальный деструктор
    // Запуск алгоритма: исследует комнаты, собирает ресурсы, возвращается в стартовую
    virtual void run(Game& game) = 0;
};
#pragma once
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// Базовый абстрактный класс ресурса
class Resource {
public:
    virtual ~Resource() = default;          // виртуальный деструктор
    virtual int getValue() const = 0;       // ценность ресурса
};

// Конкретные ресурсы – синглтоны
class Iron : public Resource {
public:
    static const Iron& instance();          // доступ к единственному экземпляру
    int getValue() const override { return 7; }
private:
    Iron() = default;                       // приватный конструктор
    ~Iron() = default;                      // деструктор по умолчанию
};

class Gold : public Resource {
public:
    static const Gold& instance();
    int getValue() const override { return 11; }
private:
    Gold() = default;
    ~Gold() = default;
};

class Gems : public Resource {
public:
    static const Gems& instance();
    int getValue() const override { return 23; }
private:
    Gems() = default;
    ~Gems() = default;
};

class Exp : public Resource {
public:
    static const Exp& instance();
    int getValue() const override { return 1; }
private:
    Exp() = default;
    ~Exp() = default;
};

// Преобразование строки в ресурс (бросает std::runtime_error)
const Resource& getResourceByName(const std::string& name);
#pragma once
#include <string>
#include <vector>
#include "room.h"
#include "resource.h"

struct ParsedData {
    std::vector<Room> rooms;
    int food;                     // M Ц количество еды
    const Resource* targetResource; // целевой ресурс (удвоенна€ ценность)
    bool success;
};

// ѕарсит входной файл, в случае ошибки бросает std::runtime_error или std::out_of_range
ParsedData parseInputFile(const std::string& filename);
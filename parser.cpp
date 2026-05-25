#include "parser.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <vector>
#include <string>

static std::string trim(const std::string& s) noexcept {
    size_t l = s.find_first_not_of(" \t\r\n");
    if (l == std::string::npos) return "";
    size_t r = s.find_last_not_of(" \t\r\n");
    return s.substr(l, r - l + 1);
}

static bool isValidNeighborsString(const std::string& s) noexcept {
    for (char c : s) {
        if (!(std::isdigit(c) || c == ',' || c == ' ' || c == '-')) return false;
    }
    return true;
}

ParsedData parseInputFile(const std::string& filename) {
    ParsedData result;
    result.success = false;
    result.targetResource = nullptr;

    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);

    int N;
    file >> N;
    if (N < 1 || N > 255) throw std::out_of_range("N out of range [1,255]");
    file.ignore();

    std::vector<Room> loadedRooms;
    loadedRooms.reserve(N + 1);

    for (int i = 0; i <= N; ++i) {
        std::string line;
        if (!std::getline(file, line))
            throw std::runtime_error("Unexpected end of file while reading room " + std::to_string(i));
        if (line.empty())
            throw std::runtime_error("Empty line while reading room " + std::to_string(i));

        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token) tokens.push_back(token);
        if (tokens.size() < 2) throw std::runtime_error(line);

        int id;
        try { id = std::stoi(tokens[0]); }
        catch (...) { throw std::runtime_error(line); }

        std::string neighborsStr = tokens[1];
        if (!isValidNeighborsString(neighborsStr)) throw std::runtime_error(line);

        std::vector<int> neighbors;
        std::stringstream ns(neighborsStr);
        std::string nbToken;
        while (std::getline(ns, nbToken, ',')) {
            nbToken = trim(nbToken);
            if (!nbToken.empty()) {
                try {
                    int nb = std::stoi(nbToken);
                    if (nb < 0 || nb > N) throw std::out_of_range("");
                    neighbors.push_back(nb);
                }
                catch (...) { throw std::runtime_error(line); }
            }
        }

        int iron = 0, gold = 0, gems = 0, exp = 0;
        if (tokens.size() > 6) throw std::runtime_error(line);
        if (tokens.size() >= 3) iron = std::stoi(tokens[2]);
        if (tokens.size() >= 4) gold = std::stoi(tokens[3]);
        if (tokens.size() >= 5) gems = std::stoi(tokens[4]);
        if (tokens.size() >= 6) exp = std::stoi(tokens[5]);

        if (iron < 0 || iron > 255 || gold < 0 || gold > 255 ||
            gems < 0 || gems > 255 || exp < 0 || exp > 255)
            throw std::runtime_error(line);

        loadedRooms.emplace_back(id, neighbors, iron, gold, gems, exp);
    }

    std::sort(loadedRooms.begin(), loadedRooms.end(),
        [](const Room& a, const Room& b) { return a.getId() < b.getId(); });
    if (loadedRooms.size() != N + 1 || loadedRooms[0].getId() != 0 || loadedRooms[N].getId() != N)
        throw std::runtime_error("Missing some room ids in range 0.." + std::to_string(N));

    std::string lastLine;
    if (!std::getline(file, lastLine) || lastLine.empty())
        if (!std::getline(file, lastLine)) throw std::runtime_error("Missing line with M and target resource");
    if (lastLine.empty()) throw std::runtime_error("Empty line for M and target resource");

    std::stringstream lastSS(lastLine);
    int M;
    std::string targetStr;
    if (!(lastSS >> M >> targetStr)) throw std::runtime_error(lastLine);
    if (M < 2 || M > 255) throw std::out_of_range("M out of range [2,255] in line: " + lastLine);

    result.targetResource = &getResourceByName(targetStr);
    result.rooms = std::move(loadedRooms);
    result.food = M;
    result.success = true;
    return result;
}
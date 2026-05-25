#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>

// RAII-класс для перенаправления std::cout в файл
class OutputRedirector {
public:
    explicit OutputRedirector(const std::string& filename) {
        file_.open(filename);
        if (!file_.is_open()) {
            throw std::runtime_error("Cannot open output file: " + filename);
        }
        originalBuf_ = std::cout.rdbuf();
        std::cout.rdbuf(file_.rdbuf());
    }

    ~OutputRedirector() {
        std::cout.rdbuf(originalBuf_);
        file_.close();
    }

    OutputRedirector(const OutputRedirector&) = delete;
    OutputRedirector& operator=(const OutputRedirector&) = delete;

private:
    std::ofstream file_;
    std::streambuf* originalBuf_;
};
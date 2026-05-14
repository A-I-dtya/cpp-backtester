#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Bar.hpp"

inline std::vector<Bar> read_csv(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::vector<Bar> bars;
    std::string line;

    if (!std::getline(file, line))
    {
        throw std::runtime_error("File is empty: " + path);
    }

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string field;
        Bar bar;

        std::getline(ss, bar.date, ',');
        std::getline(ss, field, ',');
        bar.open = std::stod(field);
        std::getline(ss, field, ',');
        bar.high = std::stod(field);
        std::getline(ss, field, ',');
        bar.low = std::stod(field);
        std::getline(ss, field, ',');
        bar.close = std::stod(field);
        std::getline(ss, field, ',');
        bar.volume = std::stoll(field);

        bars.push_back(bar);
    }

    return bars;
}
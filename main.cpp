#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>
#include "optimizer.hpp"

int main() {
    std::vector<Node> cities;
    std::ifstream file("nodes.csv");

    if (!file.is_open()) {
        std::cerr << "Error: Could not open nodes.csv" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string name, latStr, lonStr, weightStr;

        std::getline(ss, name,      ',');
        std::getline(ss, latStr,    ',');
        std::getline(ss, lonStr,    ',');
        std::getline(ss, weightStr, ',');

        cities.push_back(Node(std::stod(latStr),
                              std::stod(lonStr),
                              std::stod(weightStr)));
        std::cout << "Loaded: " << name << " ("
                  << latStr << ", " << lonStr
                  << ") weight: " << weightStr << std::endl;
    }
    file.close();

    if (cities.empty()) {
        std::cerr << "Error: No cities loaded" << std::endl;
        return 1;
    }

    std::cout << "\nTotal cities loaded: " << cities.size()
              << "\nStarting optimization...\n" << std::endl;

    Optimizer optimizer(cities);

    auto start = std::chrono::high_resolution_clock::now();
    Server best = optimizer.find_optimal_server();
    auto end   = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "=== Results ===" << std::endl;
    std::cout << "  Latitude:  " << best.lat << "°" << std::endl;
    std::cout << "  Longitude: " << best.lon << "°" << std::endl;
    std::cout << "  Cost:      " << best.cost << " km" << std::endl;
    std::cout << "  Time:      " << duration.count() << " microseconds" << std::endl;

    return 0;
}
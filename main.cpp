#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>

struct City {
    int id;
    double x;
    double y;
};

std::vector<City> parseTSPFile(const std::string &filename) {
    std::vector<City> cities;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return cities;
    }

    std::string line;
    bool readingNodes = false;

    while (std::getline(file, line)) {
        if (line == "NODE_COORD_SECTION") {
            readingNodes = true;
            continue;
        }

        if (readingNodes && line == "TOUR_SECTION") {
            break;
        }

        if (readingNodes) {
            std::istringstream iss(line);
            City city{};
            iss >> city.id >> city.x >> city.y;
            cities.push_back(city);
        }
    }

    file.close();
    return cities;
}


double calculateDistance(const City &city1, const City &city2) {
    double dx = city1.x - city2.x;
    double dy = city1.y - city2.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::pair<std::vector<City>, double> nearestNeighbor(const std::vector<City> &cities) {
    int numCities = cities.size();

    // Mark all cities as not visited
    std::vector<bool> visited(numCities, false);

    // Start from the first city
    int currentCity = 0;
    visited[currentCity] = true;

    // Result vector to store the order of visited cities
    std::vector<City> result;
    result.push_back(cities[currentCity]);

    // Total cost of the tour
    double totalCost = 0.0;

    // Continue until all cities are visited
    while (result.size() < numCities) {
        double minDistance = std::numeric_limits<double>::infinity(); // Set to positive infinity
        int nearestCity = -1;

        // Find the nearest unvisited city
        for (int i = 0; i < numCities; ++i) {
            if (!visited[i]) {
                double distance = calculateDistance(cities[currentCity], cities[i]);
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestCity = i;
                }
            }
        }

        // Move to the nearest city
        currentCity = nearestCity;
        visited[currentCity] = true;
        result.push_back(cities[currentCity]);

        // Update the total cost
        totalCost += minDistance;
    }

    // Add the cost of returning to the starting city to complete the tour
    totalCost += calculateDistance(result.back(), result.front());

    return {result, totalCost};
}

double calculateTourCost(const std::vector<City> &tour) {
    double totalCost = 0.0;
    int numCities = tour.size();

    for (int i = 0; i < numCities - 1; ++i) {
        totalCost += calculateDistance(tour[i], tour[i + 1]);
    }

    // Add the cost of returning to the starting city to complete the tour
    totalCost += calculateDistance(tour.back(), tour.front());

    return totalCost;
}

std::vector<City> orderByCities(const std::vector<City> &cities, const std::vector<int> &order) {
    std::vector<City> orderedCities;
    for (int cityId: order) {
        auto it = std::find_if(cities.begin(), cities.end(), [cityId](const City &city) {
            return city.id == cityId;
        });

        if (it != cities.end()) {
            orderedCities.push_back(*it);
        } else {
            std::cerr << "City with ID " << cityId << " not found." << std::endl;
        }
    }
    return orderedCities;
}

int main() {
    std::string filename = R"(D:\Projects\CLionProjects\tsp-nearest-neighbour\att48.tsp)";
    std::vector<City> cities = parseTSPFile(filename);

//    auto result = nearestNeighbor(cities);
//    std::vector<City> tour = result.first;
//    double totalCost = result.second;
//
//    std::cout << "Nearest Neighbor Tour:" << std::endl;
//    for (const auto& city : tour) {
//        std::cout << "ID: " << city.id << ", X: " << city.x << ", Y: " << city.y << std::endl;
//    }
//
//    std::cout << "Total Tour Cost: " << totalCost << std::endl;

    std::vector<int> specificOrder = {1,
                                      8,
                                      38,
                                      31,
                                      44,
                                      18,
                                      7,
                                      28,
                                      6,
                                      37,
                                      19,
                                      27,
                                      17,
                                      43,
                                      30,
                                      36,
                                      46,
                                      33,
                                      20,
                                      47,
                                      21,
                                      32,
                                      39,
                                      48,
                                      5,
                                      42,
                                      24,
                                      10,
                                      45,
                                      35,
                                      4,
                                      26,
                                      2,
                                      29,
                                      34,
                                      41,
                                      16,
                                      22,
                                      3,
                                      23,
                                      14,
                                      25,
                                      13,
                                      11,
                                      12,
                                      15,
                                      40,
                                      9,
                                      1};
    std::vector<City> orderedCities = orderByCities(cities, specificOrder);

    // Calculate the cost of the tour and print it
    double tourCost = calculateTourCost(orderedCities);
    std::cout << "Calculated Tour Cost: " << tourCost << std::endl;

    return 0;
}




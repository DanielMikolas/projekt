#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "Set.h"
#include "Dataset.h"

class CMeansDiscretization {
    int numIntervals_;
    std::vector<Set> finalPartitions_;
    double fuzziness_; // Fuzziness parameter
public:
    void init(Dataset& ds, int numIntervals, double fuzziness = 2.0);
    std::vector<Set> execute(Set& set, int numIntervals);
private:
    void assignToClusters(const std::vector<Point>& centroids, std::vector<Set>& clusters, Set& set, double fuzziness);
    void updateCentroids(std::vector<Point>& centroids, std::vector<Set>& clusters, const std::vector<Point>& attributes, std::mt19937& gen, std::uniform_int_distribution<>& dis, double fuzziness);
    double calculateDistance(double a, double b);
    double calculateMembership(double distance, const std::vector<Point>& centroids, size_t clusterIdx, double fuzziness);
};

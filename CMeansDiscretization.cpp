#include "CMeansDiscretization.h"
#include "Dataset.h"
#include <iostream>
#include <random>
#include <limits>
#include <cmath>
#include <algorithm>

// Hlavná funkcia pre diskretizáciu datasetu pomocou Fuzzy C-Means
std::vector<Set> CMeansDiscretization::execute(Set& set, int numIntervals) {
    set.sort(); // Zoradíme body podľa hodnoty

    const std::vector<Point>& attributes = set.getAttributes();
    std::vector<Point> centroids(numIntervals);

    // Inicializácia centroidov náhodným výberom
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, attributes.size() - 1);

    for (int i = 0; i < numIntervals; ++i) {
        centroids[i] = attributes[dis(gen)];
    }

    std::vector<Set> clusters(numIntervals);
    bool converged = false;

    while (!converged) {
        assignToClusters(centroids, clusters, set, fuzziness_);

        std::vector<Point> newCentroids(numIntervals);
        updateCentroids(newCentroids, clusters, attributes, gen, dis, fuzziness_);

        converged = true;
        for (int i = 0; i < numIntervals; ++i) {
            if (centroids[i].value != newCentroids[i].value) {
                converged = false;
                break;
            }
        }
        centroids = newCentroids;
    }

    std::sort(clusters.begin(), clusters.end(), [](Set& a, Set& b) {
        return a.getMin() < b.getMin();
    });

    for (size_t i = 0; i < clusters.size() - 1; i++) {
        double midpoint = (clusters[i].getAttributes().back().value + clusters[i + 1].getAttributes().front().value) / 2;
        clusters[i].setMax(midpoint);
        clusters[i + 1].setMin(midpoint);
    }

    return clusters;
}

void CMeansDiscretization::assignToClusters(const std::vector<Point>& centroids, std::vector<Set>& clusters, Set& set, double fuzziness) {
    for (auto& cluster : clusters) cluster.getAttributes().clear();

    for (const auto& point : set.getAttributes()) {
        double totalMembership = 0.0;
        std::vector<double> memberships(centroids.size());

        for (size_t i = 0; i < centroids.size(); i++) {
            double distance = calculateDistance(point.value, centroids[i].value);
            memberships[i] = calculateMembership(distance, centroids, i, fuzziness);
            totalMembership += memberships[i];
        }

        // Priradíme bod podľa jeho fuzzy členstva
        for (size_t i = 0; i < centroids.size(); i++) {
            if (memberships[i] > 0.5) {  // Bod bude priradený k tomuto klastru, ak je jeho členstvo viac ako 50%
                clusters[i].getAttributes().push_back(point);
            }
        }
    }
}

void CMeansDiscretization::updateCentroids(std::vector<Point>& centroids, std::vector<Set>& clusters, const std::vector<Point>& attributes, std::mt19937& gen, std::uniform_int_distribution<>& dis, double fuzziness) {
    for (size_t i = 0; i < centroids.size(); i++) {
        if (clusters[i].getAttributes().empty()) {
            // Ak je klaster prázdny, preinicializujeme centroid náhodným bodom
            centroids[i] = attributes[dis(gen)];
        } else {
            double numerator = 0.0;
            double denominator = 0.0;
            for (const Point& p : clusters[i].getAttributes()) {
                double distance = calculateDistance(p.value, centroids[i].value);
                double membership = calculateMembership(distance, centroids, i, fuzziness);
                numerator += membership * p.value;
                denominator += membership;
            }
            centroids[i].value = numerator / denominator;
        }
    }
}

double CMeansDiscretization::calculateDistance(double a, double b) {
    return std::abs(a - b);
}

double CMeansDiscretization::calculateMembership(double distance, const std::vector<Point>& centroids, size_t clusterIdx, double fuzziness) {
    double sum = 0.0;
    for (size_t j = 0; j < centroids.size(); j++) {
        double distToCentroid = calculateDistance(distance, centroids[j].value);
        double ratio = distance / distToCentroid;
        sum += std::pow(ratio, 2.0 / (fuzziness - 1));
    }
    return 1.0 / sum;
}

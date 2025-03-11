#include <iostream>
#include "Loader.h"
#include "KMeansDiscretization.h"
#include "CMeansDiscretization.h"

//dvaaplusdvajeosem
int main() {
    Loader loader;
    std::string filepath = "C:/skola/ing/discretization/Iris.txt"; // Nahraď názvom svojho súboru
    std::vector<Set> sets = loader.load(filepath, ',');

    int numIntervals = 3; // Počet intervalov (môžeš nastaviť podľa potreby)
    double fuzziness = 2.0; // Nastavenie fuzziness parametra pre C-Means

    // Výber metódy diskrétnizácie
    int choice;
    std::cout << "Zvoľte metódu diskrétnizácie:" << std::endl;
    std::cout << "1. K-Means" << std::endl;
    std::cout << "2. C-Means" << std::endl;
    std::cout << "Zadajte číslo (1 alebo 2): ";
    std::cin >> choice;

    if (choice == 1) {
        // Spustenie K-Means diskrétnizácie
        std::cout << "Spúšťam K-Means diskrétnizáciu..." << std::endl;
        for (size_t i = 0; i < sets.size(); i++) {
            Set& set = sets[i];

            KMeansDiscretization kmeans;
            std::vector<Set> clusters = kmeans.execute(set, numIntervals);

            // Výpis výsledkov
            std::cout << "Vysledky pre set " << i + 1 << " (K-Means):" << std::endl;
            for (size_t j = 0; j < clusters.size(); j++) {
                std::cout << " Interval " << j + 1 << " (min: " << clusters[j].getMin()
                          << ", max: " << clusters[j].getMax() << ")" << std::endl;

                for (const Point& point : clusters[j].getAttributes()) {
                    std::cout << "  Hodnota: " << point.value << ", Trieda: " << point.className << std::endl;
                }
            }
            std::cout << std::endl;
        }
    } else if (choice == 2) {
        // Spustenie C-Means diskrétnizácie
        std::cout << "Spúšťam C-Means diskrétnizáciu..." << std::endl;
        for (size_t i = 0; i < sets.size(); i++) {
            Set& set = sets[i];

            CMeansDiscretization cmeans;
            std::vector<Set> clusters = cmeans.execute(set, numIntervals);

            // Výpis výsledkov
            std::cout << "Vysledky pre set " << i + 1 << " (C-Means):" << std::endl;
            for (size_t j = 0; j < clusters.size(); j++) {
                std::cout << " Interval " << j + 1 << " (min: " << clusters[j].getMin()
                          << ", max: " << clusters[j].getMax() << ")" << std::endl;

                for (const Point& point : clusters[j].getAttributes()) {
                    std::cout << "  Hodnota: " << point.value << ", Trieda: " << point.className << std::endl;
                }
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Neplatná voľba! Zvoľte 1 pre K-Means alebo 2 pre C-Means." << std::endl;
    }

    return 0;
}

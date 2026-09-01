#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "heapsort.h"

int main() {
    std::vector<std::string> data;
    std::string word;

    // Read data
    std::ifstream file("dataset.txt");
    if (!file.is_open()) {
        std::cerr << "error opening data" << std::endl;
        return 1;
    }

    // Read words from file
    while (file >> word) {
        data.push_back(word);
    }
    file.close();

    std::cout << "Loaded the dataset" << std::endl;

    // Sort using heapsort and measure time without modifying the original vector
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> sortedData = heapSort(data);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Heapsort completed in " << duration.count() << " ms" << std::endl;

    if (sortedData.size() > 0) {
        std::cout << "First 10 sorted elements: ";
        for (int i = 0; i < 10 && i < (int)sortedData.size(); i++)
            std::cout << sortedData[i] << " ";
        std::cout << std::endl;
    } else {
        std::cout << "No data to display" << std::endl;
    }

    std::cout << "Original vector remains unchanged. Size: " << data.size() << std::endl;

    return 0;
}

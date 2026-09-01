#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "heapsort.h"
#include "avl.h"

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

    // Heap sort timing
    auto heapStart = std::chrono::high_resolution_clock::now();
    std::vector<std::string> heapSorted = heapSort(data);
    auto heapEnd = std::chrono::high_resolution_clock::now();
    auto heapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(heapEnd - heapStart);
    std::cout << "Heapsort completed in " << heapDuration.count() << " ms" << std::endl;

    // AVL sort timing
    auto avlStart = std::chrono::high_resolution_clock::now();
    std::vector<std::string> avlSorted = avlSort(data);
    auto avlEnd = std::chrono::high_resolution_clock::now();
    auto avlDuration = std::chrono::duration_cast<std::chrono::milliseconds>(avlEnd - avlStart);
    std::cout << "AVL sort completed in " << avlDuration.count() << " ms" << std::endl;

    if (heapSorted.size() > 0) {
        std::cout << "First 10 heap-sorted elements: ";
        for (int i = 0; i < 10 && i < (int)heapSorted.size(); i++)
            std::cout << heapSorted[i] << " ";
        std::cout << std::endl;
    } else {
        std::cout << "No heap-sort data to display" << std::endl;
    }

    if (avlSorted.size() > 0) {
        std::cout << "First 10 AVL-sorted elements: ";
        for (int i = 0; i < 10 && i < (int)avlSorted.size(); i++)
            std::cout << avlSorted[i] << " ";
        std::cout << std::endl;
    } else {
        std::cout << "No AVL data to display" << std::endl;
    }

    std::cout << "Original vector remains unchanged. Size: " << data.size() << std::endl;

    return 0;
}

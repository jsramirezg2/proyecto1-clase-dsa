#include "heapsort.h"
#include <vector>
#include <string>

void customSwap(std::string& a, std::string& b) {
    std::string temp = a;
    a = b;
    b = temp;
}

// Helper: maintain max heap property
void heapify(std::vector<std::string>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Find largest among root, left, and right
    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Swap and recurse if needed
    if (largest != i) {
        customSwap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Main heapsort function
std::vector<std::string> heapSort(const std::vector<std::string>& arr) {
    std::vector<std::string> sorted = arr; // work on a copy so the original is untouched
    int n = static_cast<int>(sorted.size());

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(sorted, n, i);

    // Extract elements from heap
    for (int i = n - 1; i > 0; i--) {
        customSwap(sorted[0], sorted[i]);
        heapify(sorted, i, 0);
    }

    return sorted;
}
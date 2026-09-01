#include "heapsort.h"
#include <algorithm>

// Baja el elemento i hasta su lugar para mantener el heap maximo
void heapify(std::vector<std::string>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

std::vector<std::string> heapSort(const std::vector<std::string>& arr) {
    std::vector<std::string> sorted = arr;  // copia para no tocar el original
    int n = sorted.size();

    // 1. Construir el heap maximo
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(sorted, n, i);

    // 2. Sacar el mayor y ponerlo al final, una y otra vez
    for (int i = n - 1; i > 0; i--) {
        std::swap(sorted[0], sorted[i]);
        heapify(sorted, i, 0);
    }

    return sorted;
}

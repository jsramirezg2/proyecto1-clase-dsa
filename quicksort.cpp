#include "quicksort.h"
#include <algorithm>

// Ordena el trozo del arreglo entre las posiciones left y right
void quickSortRecursive(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;

    std::string pivot = arr[(left + right) / 2];
    int i = left;
    int j = right;

    // Deja los menores al pivote a la izquierda y los mayores a la derecha
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    quickSortRecursive(arr, left, j);
    quickSortRecursive(arr, i, right);
}

std::vector<std::string> quickSort(const std::vector<std::string>& arr) {
    std::vector<std::string> sorted = arr;  // copia para no tocar el original
    if (sorted.size() > 1)
        quickSortRecursive(sorted, 0, sorted.size() - 1);
    return sorted;
}

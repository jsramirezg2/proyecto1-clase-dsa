#include "quicksort.h"
#include <vector>
#include <string>

namespace {
    void quickSortRecursive(std::vector<std::string>& arr, int left, int right) {
        if (left >= right) return;

        std::string pivot = arr[(left + right) / 2];
        int i = left;
        int j = right;

        while (i <= j) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;

            if (i <= j) {
                std::string temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
                j--;
            }
        }

        if (left < j) quickSortRecursive(arr, left, j);
        if (i < right) quickSortRecursive(arr, i, right);
    }
}

std::vector<std::string> quickSort(const std::vector<std::string>& arr) {
    std::vector<std::string> sorted = arr;
    if (sorted.size() <= 1) {
        return sorted;
    }

    quickSortRecursive(sorted, 0, static_cast<int>(sorted.size()) - 1);
    return sorted;
}

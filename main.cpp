#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include "heapsort.h"
#include "avl.h"
#include "quicksort.h"

int main() {
    // El archivo esta guardado en UTF-16: cada letra ocupa 2 bytes y el
    // segundo byte siempre es 0, asi que leemos en binario y nos quedamos
    // con el primer byte de cada par (empezando en 2 para saltar el BOM).
    std::ifstream file("dataset.txt", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "No se pudo abrir dataset.txt" << std::endl;
        return 1;
    }

    std::string bytes;
    char c;
    while (file.get(c)) {
        bytes += c;
    }
    file.close();

    std::string texto;
    for (int i = 2; i < (int)bytes.size(); i += 2) {
        texto += bytes[i];
    }

    // Separar el texto en palabras
    std::vector<std::string> data;
    std::string palabra;
    std::istringstream ss(texto);
    while (ss >> palabra) {
        data.push_back(palabra);
    }

    std::cout << "Palabras leidas: " << data.size() << std::endl << std::endl;

    // --- Heapsort ---
    auto inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::string> r1 = heapSort(data);
    auto fin = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
    std::cout << "Heapsort:  " << ms.count() << " ms" << std::endl;

    // --- AVL sort ---
    inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::string> r2 = avlSort(data);
    fin = std::chrono::high_resolution_clock::now();
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
    std::cout << "AVL sort:  " << ms.count() << " ms" << std::endl;

    // --- Quicksort ---
    inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::string> r3 = quickSort(data);
    fin = std::chrono::high_resolution_clock::now();
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
    std::cout << "Quicksort: " << ms.count() << " ms" << std::endl << std::endl;

    // Mostrar las primeras 10 palabras de cada resultado
    std::cout << "Heapsort  -> ";
    for (int i = 0; i < 10 && i < (int)r1.size(); i++) std::cout << r1[i] << " ";
    std::cout << std::endl;

    std::cout << "AVL sort  -> ";
    for (int i = 0; i < 10 && i < (int)r2.size(); i++) std::cout << r2[i] << " ";
    std::cout << std::endl;

    std::cout << "Quicksort -> ";
    for (int i = 0; i < 10 && i < (int)r3.size(); i++) std::cout << r3[i] << " ";
    std::cout << std::endl;

    return 0;
}

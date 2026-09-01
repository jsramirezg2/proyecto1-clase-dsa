#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <iomanip>
#include "heapsort.h"
#include "avl.h"
#include "quicksort.h"

// Marco aproximado de una llamada recursiva (parametros + direccion de retorno).
// Es una estimacion: sirve para ver que la pila es despreciable frente al resto.
const std::size_t MARCO_PILA = 64;

// Bytes que las palabras piden al heap, fuera del objeto std::string.
// Las cadenas cortas se guardan dentro del propio objeto (SSO) y no piden
// memoria extra, asi que solo contamos las que si la piden.
std::size_t bytesEnHeap(const std::vector<std::string>& v) {
    std::size_t sso = std::string().capacity();
    std::size_t total = 0;
    for (std::size_t i = 0; i < v.size(); i++)
        if (v[i].capacity() > sso)
            total += v[i].capacity() + 1;
    return total;
}

// Memoria total de un vector de palabras: el objeto vector, el arreglo de
// objetos std::string y los caracteres que no caben en SSO.
// Se usa size() y no capacity() para contar solo los elementos almacenados.
std::size_t memoriaVector(const std::vector<std::string>& v) {
    return sizeof(v) + v.size() * sizeof(std::string) + bytesEnHeap(v);
}

std::string kb(std::size_t bytes) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << (bytes / 1024.0) << " KB";
    return out.str();
}

void linea(const std::string& titulo) {
    std::cout << std::endl << "=== " << titulo << " ===" << std::endl;
}

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

    std::size_t n = data.size();
    std::cout << "Palabras leidas: " << n << std::endl;

    // --- Quicksort ---
    auto inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::string> r1 = quickSort(data);
    auto fin = std::chrono::high_resolution_clock::now();
    long tQuick = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();

    // --- Heapsort ---
    inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::string> r2 = heapSort(data);
    fin = std::chrono::high_resolution_clock::now();
    long tHeap = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();

    // --- AVL sort ---
    inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::string> r3 = avlSort(data);
    fin = std::chrono::high_resolution_clock::now();
    long tAvl = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();

    linea("TIEMPOS DE EJECUCION");
    std::cout << "Quicksort: " << tQuick << " ms" << std::endl;
    std::cout << "Heapsort:  " << tHeap << " ms" << std::endl;
    std::cout << "AVL:       " << tAvl << " ms" << std::endl;

    linea("PRIMERAS 10 PALABRAS ORDENADAS");
    std::cout << "Quicksort -> ";
    for (int i = 0; i < 10 && i < (int)r1.size(); i++) std::cout << r1[i] << " ";
    std::cout << std::endl << "Heapsort  -> ";
    for (int i = 0; i < 10 && i < (int)r2.size(); i++) std::cout << r2[i] << " ";
    std::cout << std::endl << "AVL       -> ";
    for (int i = 0; i < 10 && i < (int)r3.size(); i++) std::cout << r3[i] << " ";
    std::cout << std::endl;

    // ------------------------------------------------------------------
    // Estimacion de memoria con sizeof
    // ------------------------------------------------------------------
    std::size_t memDataset = memoriaVector(data);
    int profundidad = (int)std::ceil(std::log2((double)n));   // recursion de quicksort/heapify
    std::size_t nodo = avlTamanoNodo();

    std::size_t memQuick = memoriaVector(r1) + profundidad * MARCO_PILA;
    std::size_t memHeap  = memoriaVector(r2) + profundidad * MARCO_PILA;
    std::size_t memAvl   = memoriaVector(r3) + n * nodo + bytesEnHeap(data)
                           + avlUltimaAltura() * MARCO_PILA;

    linea("TAMANOS BASE (sizeof)");
    std::cout << "sizeof(std::string)         : " << sizeof(std::string) << " bytes"
              << "  (SSO: hasta " << std::string().capacity() << " caracteres sin pedir heap)" << std::endl;
    std::cout << "sizeof(std::vector<string>) : " << sizeof(std::vector<std::string>) << " bytes" << std::endl;
    std::cout << "sizeof(Nodo AVL)            : " << nodo << " bytes"
              << "  (string + 2 punteros + altura)" << std::endl;

    linea("MEMORIA ESTIMADA");
    std::cout << "Dataset original (" << n << " palabras): " << kb(memDataset) << std::endl;
    std::cout << "  = vector (" << sizeof(data) << " B) + " << n << " x " << sizeof(std::string)
              << " B + " << kb(bytesEnHeap(data)) << " de palabras largas (> "
              << std::string().capacity() << " caracteres)" << std::endl;
    std::cout << "  Nota: el vector reservo capacidad para " << data.capacity()
              << " elementos al crecer con push_back, asi que el consumo real es algo mayor."
              << std::endl << std::endl;

    std::cout << "Quicksort" << std::endl;
    std::cout << "  Estructura      : std::vector<std::string> (arreglo dinamico)" << std::endl;
    std::cout << "  Elementos       : " << n << std::endl;
    std::cout << "  Extra           : copia del vector + pila de recursion ("
              << profundidad << " niveles x " << MARCO_PILA << " B)" << std::endl;
    std::cout << "  Total estimado  : " << kb(memQuick) << std::endl << std::endl;

    std::cout << "Heapsort" << std::endl;
    std::cout << "  Estructura      : std::vector<std::string> usado como heap binario" << std::endl;
    std::cout << "  Elementos       : " << n << std::endl;
    std::cout << "  Extra           : copia del vector + pila de heapify ("
              << profundidad << " niveles x " << MARCO_PILA << " B). Ordena in-place." << std::endl;
    std::cout << "  Total estimado  : " << kb(memHeap) << std::endl << std::endl;

    std::cout << "AVL" << std::endl;
    std::cout << "  Estructura      : arbol AVL de " << n << " nodos + vector resultado" << std::endl;
    std::cout << "  Elementos       : " << n << " nodos x " << nodo << " B = "
              << kb(n * nodo) << std::endl;
    std::cout << "  Extra           : vector con el recorrido in-orden + pila del recorrido ("
              << avlUltimaAltura() << " niveles)" << std::endl;
    std::cout << "  Total estimado  : " << kb(memAvl) << std::endl;

    // ------------------------------------------------------------------
    // Analisis comparativo
    // ------------------------------------------------------------------
    linea("ANALISIS COMPARATIVO");
    std::cout << std::left
              << std::setw(12) << "Algoritmo"
              << std::setw(14) << "Promedio"
              << std::setw(14) << "Peor caso"
              << std::setw(14) << "Memoria O()"
              << std::setw(10) << "Tiempo"
              << "Memoria" << std::endl;
    std::cout << std::string(76, '-') << std::endl;
    std::cout << std::setw(12) << "Quicksort" << std::setw(14) << "O(n log n)"
              << std::setw(14) << "O(n^2)" << std::setw(14) << "O(log n)"
              << std::setw(10) << (std::to_string(tQuick) + " ms") << kb(memQuick) << std::endl;
    std::cout << std::setw(12) << "Heapsort" << std::setw(14) << "O(n log n)"
              << std::setw(14) << "O(n log n)" << std::setw(14) << "O(1)"
              << std::setw(10) << (std::to_string(tHeap) + " ms") << kb(memHeap) << std::endl;
    std::cout << std::setw(12) << "AVL" << std::setw(14) << "O(n log n)"
              << std::setw(14) << "O(n log n)" << std::setw(14) << "O(n)"
              << std::setw(10) << (std::to_string(tAvl) + " ms") << kb(memAvl) << std::endl;
    std::cout << std::right << std::endl;

    std::cout << "Los tres son O(n log n) en promedio, pero el tiempo real no es igual:" << std::endl;
    std::cout << "- Quicksort gana porque trabaja sobre un arreglo contiguo y compara" << std::endl;
    std::cout << "  elementos vecinos, asi que aprovecha la cache del procesador." << std::endl;
    std::cout << "- Heapsort tiene la misma complejidad pero salta entre las posiciones" << std::endl;
    std::cout << "  i, 2i+1 y 2i+2, que estan lejos en memoria: falla mas en cache." << std::endl;
    std::cout << "- El AVL paga " << nodo << " bytes por nodo y una asignacion de memoria" << std::endl;
    std::cout << "  por palabra, ademas de las rotaciones para mantenerse balanceado." << std::endl;
    std::cout << "  Con " << n << " palabras su altura quedo en " << avlUltimaAltura()
              << " (el minimo teorico es " << profundidad << ")." << std::endl;
    std::cout << std::endl;
    std::cout << "Por eso la complejidad teorica no basta: cuenta comparaciones, no" << std::endl;
    std::cout << "accesos a cache ni llamadas a new. En memoria la diferencia es clara:" << std::endl;
    std::cout << "quicksort y heapsort solo necesitan la copia del arreglo, mientras que" << std::endl;
    std::cout << "el AVL usa " << kb(memAvl) << " frente a " << kb(memQuick) << " de quicksort." << std::endl;
    std::cout << "La ventaja del AVL no es ordenar, sino que queda como estructura viva:" << std::endl;
    std::cout << "admite busquedas, inserciones y borrados en O(log n) despues de ordenar." << std::endl;

    return 0;
}

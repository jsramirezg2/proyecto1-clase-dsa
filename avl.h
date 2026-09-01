#ifndef AVL_H
#define AVL_H

#include <vector>
#include <string>
#include <cstddef>

std::vector<std::string> avlSort(const std::vector<std::string>& arr);

// Datos del arbol para el analisis de memoria
std::size_t avlTamanoNodo();   // sizeof de un nodo del arbol
int avlUltimaAltura();         // altura del arbol del ultimo avlSort

#endif

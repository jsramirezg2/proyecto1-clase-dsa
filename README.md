# Proyecto 1 - Algoritmos de Ordenamiento

Comparación de tres algoritmos de ordenamiento sobre un dataset de 100.000
palabras (`dataset.txt`), midiendo cuánto tarda cada uno con `std::chrono`.

## Archivos

- `main.cpp` — lee el dataset, llama a los tres algoritmos y toma los tiempos.
- `heapsort.cpp` / `heapsort.h`
- `avl.cpp` / `avl.h`
- `quicksort.cpp` / `quicksort.h`
- `dataset.txt` — las 100.000 palabras (guardado en UTF-16).

## Algoritmos

**Heapsort** — O(n log n) siempre. Arma un montículo (heap) máximo con todo el
arreglo, saca el mayor y lo pone al final, y repite con lo que queda.

**AVL sort** — O(n log n). Inserta todas las palabras en un árbol AVL (árbol
binario de búsqueda que se rota solo para mantenerse balanceado) y después lo
recorre in-orden (izquierda, raíz, derecha), lo que devuelve las palabras ya
ordenadas.

**Quicksort** — O(n log n) en promedio, O(n²) en el peor caso. Escoge un pivote
(el elemento del medio), deja los menores a la izquierda y los mayores a la
derecha, y ordena cada mitad recursivamente.

## Compilar y ejecutar

```bash
g++ -o main main.cpp heapsort.cpp avl.cpp quicksort.cpp
./main
```

## Resultados

Cada algoritmo recibe una copia del vector original, así que los tres ordenan
exactamente los mismos datos y el original no se modifica.

| Algoritmo | Tiempo |
|---|---|
| Heapsort  | ~245 ms |
| AVL sort  | ~135 ms |
| Quicksort | ~115 ms |

## Conclusión

Quicksort fue el más rápido y Heapsort el más lento, aunque los tres son
O(n log n). La diferencia está en cómo se mueven por la memoria: Quicksort
compara elementos que están cerca uno del otro, mientras que Heapsort salta
constantemente entre posiciones lejanas del arreglo, lo que lo hace más lento
en la práctica. El AVL queda en el medio, pero es el que más memoria gasta
porque crea un nodo con dos punteros por cada palabra.

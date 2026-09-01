### NOMBRES :
- Miguel Perez
- Roger Marquez
- Juan Sebastian Ramirez G.


# Proyecto 1 - Ordenamiento de un dataset grande (C++)

Comparación de tres estrategias de ordenamiento sobre un dataset de 100.000
palabras en inglés (`dataset.txt`), midiendo el tiempo real con `std::chrono` y
estimando el consumo de memoria con `sizeof`.

## Archivos

- `main.cpp` — lee el dataset, llama a los tres algoritmos, toma los tiempos y
  hace el análisis de memoria y la comparación final.
- `quicksort.cpp` / `quicksort.h` — QuickSort sobre `std::vector<std::string>`.
- `heapsort.cpp` / `heapsort.h` — HeapSort con heap binario máximo.
- `avl.cpp` / `avl.h` — árbol AVL con recorrido in-orden.
- `dataset.txt` — las 100.000 palabras desordenadas (guardado en UTF-16).

No se usa ninguna función de ordenamiento de la librería estándar: `std::sort`
no aparece en el proyecto. Solo se usan `std::swap` y `std::max` como utilidades.

## Compilar y ejecutar

```bash
g++ -o main main.cpp heapsort.cpp avl.cpp quicksort.cpp
./main
```

Los tiempos de este README se midieron con esa compilación (sin `-O2`). Con
`-O2` los tres bajan a menos de la mitad, pero la comparación relativa cambia,
así que conviene medir siempre con las mismas banderas.

## Enfoque de implementación

**QuickSort** — Implementado desde cero sobre un arreglo dinámico
(`std::vector<std::string>`). Usa partición de Hoare con el elemento del medio
como pivote: recorre desde los dos extremos, intercambia los elementos que están
del lado equivocado y luego ordena las dos mitades recursivamente. Elegir el
pivote del medio y no el primero evita el peor caso O(n²) cuando la entrada ya
viene ordenada, que es justo el caso del archivo original `words_alpha.txt`.

**HeapSort** — Construye un heap binario máximo dentro del mismo vector
(el hijo izquierdo de `i` está en `2i+1` y el derecho en `2i+2`). Primero
`heapify` de abajo hacia arriba para armar el heap en O(n), y después n
extracciones: intercambia la raíz (el mayor) con el último elemento del heap,
reduce el heap en uno y vuelve a hundir la raíz. Ordena in-place.

**AVL** — Árbol binario de búsqueda balanceado. Cada nodo guarda su altura y,
después de cada inserción, se calcula el factor de balance (altura izquierda −
altura derecha); si queda fuera del rango [-1, 1] se aplica la rotación que
corresponda (LL, RR, LR o RL). Las palabras repetidas se insertan a la derecha
para no perder ninguna. Se insertan las 100.000 palabras, se recorre el árbol
in-orden (izquierda → raíz → derecha) y ese recorrido se guarda en un
`std::vector<std::string>`, que queda ordenado alfabéticamente. Al terminar se
libera el árbol.

### Lectura del dataset

`dataset.txt` está guardado en UTF-16 little-endian: cada letra ocupa 2 bytes y
el segundo siempre es 0 (son palabras ASCII). Por eso `main.cpp` abre el archivo
en binario, salta los 2 bytes del BOM y se queda con el primer byte de cada par.
Luego separa el texto en palabras con `std::istringstream`.

## Medición de rendimiento

El tiempo se mide con `std::chrono::high_resolution_clock` alrededor de cada
llamada, e incluye el proceso completo de cada estrategia (para el AVL: las
100.000 inserciones + el recorrido in-orden). Los tres algoritmos reciben una
copia del vector original, así que ordenan exactamente los mismos datos y
ninguno se beneficia del trabajo del anterior.

| Algoritmo | Tiempo |
|---|---|
| QuickSort | ~115 ms |
| AVL       | ~120 ms |
| HeapSort  | ~243 ms |

## Estimación de memoria

El programa calcula la memoria con `sizeof` sobre las estructuras reales:

| Tipo | Tamaño |
|---|---|
| `std::string` | 32 bytes (con SSO: hasta 15 caracteres sin pedir heap) |
| `std::vector<std::string>` | 24 bytes (3 punteros) |
| `Nodo` del AVL | 56 bytes (string 32 + 2 punteros 16 + altura 4 + relleno 4) |

Para un vector de palabras la cuenta es:

```
memoria = sizeof(vector) + n * sizeof(std::string) + caracteres en el heap
```

El último término solo cuenta las palabras de más de 15 caracteres, porque las
cortas se guardan dentro del propio objeto `std::string` (*small string
optimization*) y no piden memoria aparte. En este dataset eso son unos 53 KB.

| Algoritmo | Estructura | Elementos | Estructuras adicionales | Total |
|---|---|---|---|---|
| QuickSort | arreglo dinámico | 100.000 | copia del vector + pila de recursión (17 niveles) | ~3.179 KB |
| HeapSort  | arreglo dinámico usado como heap | 100.000 | copia del vector + pila de `heapify` (17 niveles) | ~3.179 KB |
| AVL       | árbol de 100.000 nodos + vector resultado | 100.000 nodos × 56 B | vector del recorrido in-orden + pila del recorrido (20 niveles) | ~8.702 KB |

La pila de recursión se estima en unos 64 bytes por marco; con profundidad
logarítmica son ~1 KB, despreciable frente al resto.

## Análisis comparativo

| Algoritmo | Promedio | Peor caso | Memoria extra | Tiempo real | Memoria |
|---|---|---|---|---|---|
| QuickSort | O(n log n) | O(n²) | O(log n) | ~115 ms | ~3.179 KB |
| HeapSort  | O(n log n) | O(n log n) | O(1) | ~243 ms | ~3.179 KB |
| AVL       | O(n log n) | O(n log n) | O(n) | ~120 ms | ~8.702 KB |

### ¿Cuál algoritmo se comportó mejor?

QuickSort. Fue el más rápido y el que menos memoria usa, empatado con HeapSort.
El AVL quedó muy cerca en tiempo, pero gastando casi tres veces más memoria.
HeapSort fue el más lento, más del doble que QuickSort.

### ¿Por qué la complejidad teórica no coincide con los resultados prácticos?

Porque Big O cuenta comparaciones e intercambios, y supone que todos los accesos
a memoria cuestan lo mismo. En un procesador real no es así:

- **QuickSort** recorre el arreglo de forma secuencial y compara elementos que
  están juntos en memoria, así que casi siempre encuentra el dato en la caché.
- **HeapSort** salta entre las posiciones `i`, `2i+1` y `2i+2`, que con 100.000
  elementos están muy lejos unas de otras. Cada salto es un fallo de caché, y
  por eso tarda el doble aunque haga la misma cantidad de comparaciones.
- El **AVL** hace un `new` por palabra (100.000 asignaciones de memoria) y
  rotaciones para mantenerse balanceado; ese costo no aparece en el O(n log n).
  Además sus nodos quedan dispersos por el heap, así que el recorrido in-orden
  tampoco aprovecha la caché.

También influyen las constantes ocultas: comparar dos `std::string` es mucho más
caro que comparar dos enteros, y HeapSort hace más intercambios que QuickSort.

### Ventajas y desventajas de cada estructura

**Arreglo dinámico con QuickSort** — Ventajas: el más rápido, memoria contigua,
casi sin memoria extra. Desventajas: el peor caso es O(n²) si el pivote se elige
mal, no es estable, y la recursión puede desbordar la pila con entradas
adversas.

**Heap binario** — Ventajas: O(n log n) garantizado sin importar la entrada,
ordena in-place y sirve como cola de prioridad (sacar el máximo en O(log n)).
Desventajas: el patrón de acceso es malo para la caché, no es estable y en la
práctica es el más lento de los tres.

**Árbol AVL** — Ventajas: no es solo un algoritmo de ordenamiento, sino una
estructura que queda viva: permite buscar, insertar y borrar en O(log n)
manteniendo el orden. Desventajas: 56 bytes por nodo más una asignación de
memoria por palabra, casi tres veces más memoria que las otras dos opciones, y
el costo de las rotaciones. Con estas 100.000 palabras la altura quedó en 20,
frente al mínimo teórico de 17: el balanceo funciona bien.

## Conclusión

Si el objetivo es simplemente ordenar una vez y ya, **QuickSort es la mejor
opción para este problema**: el menor tiempo y el menor consumo de memoria. Vale
la pena cuidar la elección del pivote, que es su único punto débil.

Si hace falta una garantía dura de O(n log n) sin depender del pivote y con
memoria mínima, HeapSort es la alternativa segura, aceptando que es más lento.

El AVL solo se justifica si después de ordenar hay que seguir consultando o
modificando el conjunto: pagar 8,7 MB y las rotaciones tiene sentido cuando eso
compra búsquedas e inserciones en O(log n). Como algoritmo de ordenamiento
puntual, es gastar memoria de más para conseguir un tiempo que QuickSort ya
consigue.

#include "avl.h"
#include <algorithm>

struct Nodo {
    std::string valor;
    Nodo* izq;
    Nodo* der;
    int altura;

    Nodo(const std::string& v) {
        valor = v;
        izq = nullptr;
        der = nullptr;
        altura = 1;
    }
};

int altura(Nodo* n) {
    if (n == nullptr) return 0;
    return n->altura;
}

// Diferencia de altura entre el lado izquierdo y el derecho
int balance(Nodo* n) {
    if (n == nullptr) return 0;
    return altura(n->izq) - altura(n->der);
}

void actualizarAltura(Nodo* n) {
    n->altura = 1 + std::max(altura(n->izq), altura(n->der));
}

Nodo* rotarDerecha(Nodo* y) {
    Nodo* x = y->izq;
    y->izq = x->der;
    x->der = y;

    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

Nodo* rotarIzquierda(Nodo* x) {
    Nodo* y = x->der;
    x->der = y->izq;
    y->izq = x;

    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

Nodo* insertar(Nodo* nodo, const std::string& valor) {
    if (nodo == nullptr) return new Nodo(valor);

    // Los repetidos se van a la derecha para no perder ninguna palabra
    if (valor < nodo->valor)
        nodo->izq = insertar(nodo->izq, valor);
    else
        nodo->der = insertar(nodo->der, valor);

    actualizarAltura(nodo);

    // Si quedo desbalanceado, se rota para volver a equilibrarlo
    int b = balance(nodo);

    if (b > 1 && valor < nodo->izq->valor)
        return rotarDerecha(nodo);

    if (b < -1 && valor >= nodo->der->valor)
        return rotarIzquierda(nodo);

    if (b > 1 && valor >= nodo->izq->valor) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (b < -1 && valor < nodo->der->valor) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// Recorrido in-orden: izquierda, raiz, derecha -> sale ordenado
void inOrden(Nodo* nodo, std::vector<std::string>& resultado) {
    if (nodo == nullptr) return;

    inOrden(nodo->izq, resultado);
    resultado.push_back(nodo->valor);
    inOrden(nodo->der, resultado);
}

void borrarArbol(Nodo* nodo) {
    if (nodo == nullptr) return;
    borrarArbol(nodo->izq);
    borrarArbol(nodo->der);
    delete nodo;
}

std::vector<std::string> avlSort(const std::vector<std::string>& arr) {
    Nodo* raiz = nullptr;
    for (int i = 0; i < (int)arr.size(); i++)
        raiz = insertar(raiz, arr[i]);

    std::vector<std::string> resultado;
    inOrden(raiz, resultado);
    borrarArbol(raiz);
    return resultado;
}

#ifndef COBERTURA_H
#define COBERTURA_H

#include <stdbool.h>

typedef struct {
    int u, v;
} Aresta;

/* Grafo nao direcionado, com vertices numerados de 0 a n-1.
 * Os vetores de selecao tem n posicoes. arestas pode ser NULL se m for zero.
 * Os algoritmos retornam o tamanho da cobertura ou -1 se faltar memoria. */
bool eh_cobertura(const Aresta *arestas, int m, const bool *selecao);
int cobertura_minima_bt(int n, const Aresta *arestas, int m, bool *selecao);
int cobertura_gulosa(int n, const Aresta *arestas, int m, bool *selecao);

#endif

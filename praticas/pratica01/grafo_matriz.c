#include <stdlib.h>

#include "grafo_matriz.h"

static int vertice_valido(const GrafoMatriz *g, int vertice)
{
    return g != NULL && vertice >= 0 && vertice < g->n;
}

GrafoMatriz *criar_grafo_matriz(int n)
{
    if (n < 0) {
        return NULL;
    }

    GrafoMatriz *g = malloc(sizeof(GrafoMatriz));
    if (g == NULL) {
        return NULL;
    }

    g->n = n;
    g->adj = NULL;

    if (n == 0) {
        return g;
    }

    g->adj = malloc((size_t)n * sizeof(int *));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g->adj[i] = calloc((size_t)n, sizeof(int));

        if (g->adj[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(g->adj[j]);
            }

            free(g->adj);
            free(g);
            return NULL;
        }
    }

    return g;
}

int inserir_aresta_matriz(GrafoMatriz *g, int origem, int destino)
{
    if (!vertice_valido(g, origem) || !vertice_valido(g, destino)) {
        return -1;
    }

    /*
     * Grafo simples:
     * nao permite laco.
     */
    if (origem == destino) {
        return -1;
    }

    /*
     * Nao permite aresta duplicada.
     */
    if (g->adj[origem][destino] == 1) {
        return -1;
    }

    /*
     * Grafo nao direcionado:
     * origem -> destino
     * destino -> origem
     */
    g->adj[origem][destino] = 1;
    g->adj[destino][origem] = 1;

    return 0;
}

int remover_aresta_matriz(GrafoMatriz *g, int origem, int destino)
{
    if (!vertice_valido(g, origem) || !vertice_valido(g, destino)) {
        return -1;
    }

    if (g->adj[origem][destino] == 0) {
        return -1;
    }

    g->adj[origem][destino] = 0;
    g->adj[destino][origem] = 0;

    return 0;
}

int grau_matriz(const GrafoMatriz *g, int vertice)
{
    if (!vertice_valido(g, vertice)) {
        return -1;
    }

    int grau = 0;

    for (int i = 0; i < g->n; i++) {
        grau += g->adj[vertice][i];
    }

    return grau;
}

int sao_adjacentes_matriz(const GrafoMatriz *g, int origem, int destino)
{
    if (!vertice_valido(g, origem) || !vertice_valido(g, destino)) {
        return -1;
    }

    return g->adj[origem][destino];
}

void liberar_grafo_matriz(GrafoMatriz *g)
{
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }

    free(g->adj);
    free(g);
}
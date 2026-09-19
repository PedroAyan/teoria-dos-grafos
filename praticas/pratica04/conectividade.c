#include "conectividade.h"
#include <stdlib.h>

GrafoLista *criar_grafo_lista(int n)
{
    if (n < 0) return NULL;
    GrafoLista *g = malloc(sizeof *g);
    if (!g) return NULL;
    g->n = n;
    g->adj = calloc((size_t)(n ? n : 1), sizeof *g->adj);
    if (!g->adj) { free(g); return NULL; }
    return g;
}

int inserir_aresta_lista(GrafoLista *g, int origem, int destino)
{
    if (!g || origem < 0 || destino < 0 || origem >= g->n ||
        destino >= g->n || origem == destino) return -1;
    for (No *a = g->adj[origem]; a; a = a->prox)
        if (a->destino == destino) return -1;
    No *a = malloc(sizeof *a), *b = malloc(sizeof *b);
    if (!a || !b) { free(a); free(b); return -1; }
    *a = (No){destino, g->adj[origem]};
    *b = (No){origem, g->adj[destino]};
    g->adj[origem] = a;
    g->adj[destino] = b;
    return 0;
}

void liberar_grafo_lista(GrafoLista *g)
{
    if (!g) return;
    for (int u = 0; u < g->n; ++u) {
        No *a = g->adj[u];
        while (a) { No *proximo = a->prox; free(a); a = proximo; }
    }
    free(g->adj);
    free(g);
}

void dfs_articulacoes(const GrafoLista *g, int u, int pai, int *tempo,
                     int descoberta[], int low[], int articulacao[],
                     Ponte pontes[], int *quantidade)
{
    descoberta[u] = low[u] = ++(*tempo);
    int filhos = 0;
    for (No *a = g->adj[u]; a; a = a->prox) {
        int v = a->destino;
        if (!descoberta[v]) {
            ++filhos;
            dfs_articulacoes(g, v, u, tempo, descoberta, low,
                             articulacao, pontes, quantidade);
            if (low[v] < low[u]) low[u] = low[v];
            if (pai != -1 && low[v] >= descoberta[u]) articulacao[u] = 1;
            if (pontes && low[v] > descoberta[u])
                pontes[(*quantidade)++] = (Ponte){u, v};
        } else if (v != pai && descoberta[v] < low[u]) {
            low[u] = descoberta[v];
        }
    }
    if (pai == -1 && filhos > 1) articulacao[u] = 1;
}

static int analisar(const GrafoLista *g, int articulacao[], Ponte pontes[])
{
    size_t n = (size_t)(g->n ? g->n : 1);
    int *descoberta = calloc(n, sizeof *descoberta);
    int *low = calloc(n, sizeof *low);
    if (!descoberta || !low) { free(descoberta); free(low); return -1; }
    for (int u = 0; u < g->n; ++u) articulacao[u] = 0;
    int tempo = 0, quantidade = 0;
    for (int u = 0; u < g->n; ++u)
        if (!descoberta[u])
            dfs_articulacoes(g, u, -1, &tempo, descoberta, low,
                             articulacao, pontes, &quantidade);
    free(descoberta);
    free(low);
    return quantidade;
}

int detectar_articulacoes(const GrafoLista *g, int articulacao[])
{
    if (!g || !articulacao) return -1;
    if (analisar(g, articulacao, NULL) < 0) return -1;
    int quantidade = 0;
    for (int u = 0; u < g->n; ++u) quantidade += articulacao[u];
    return quantidade;
}

int detectar_pontes(const GrafoLista *g, Ponte pontes[])
{
    if (!g || !pontes) return -1;
    int *articulacao = calloc((size_t)(g->n ? g->n : 1), sizeof *articulacao);
    if (!articulacao) return -1;
    int quantidade = analisar(g, articulacao, pontes);
    free(articulacao);
    return quantidade;
}

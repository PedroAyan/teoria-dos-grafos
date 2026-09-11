#include "dag.h"
#include <stdlib.h>

GrafoLista *criar_grafo_lista(int n)
{
    if (n < 0) return NULL;
    GrafoLista *g = malloc(sizeof *g);
    if (!g) return NULL;
    g->n = n;
    g->adj = calloc((size_t)(n > 0 ? n : 1), sizeof *g->adj);
    if (!g->adj) {
        free(g);
        return NULL;
    }
    return g;
}

int inserir_aresta_lista(GrafoLista *g, int origem, int destino)
{
    if (!g || origem < 0 || destino < 0 || origem >= g->n || destino >= g->n)
        return 0;
    No *novo = malloc(sizeof *novo);
    if (!novo) return 0;
    novo->destino = destino;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo; /* Aresta dirigida: nao inserir a inversa. */
    return 1;
}

void liberar_grafo_lista(GrafoLista *g)
{
    if (!g) return;
    for (int v = 0; v < g->n; ++v) {
        No *atual = g->adj[v];
        while (atual) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(g->adj);
    free(g);
}

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho)
{
    if (!tamanho) return NULL;
    *tamanho = 0;
    if (!g || g->n < 0) return NULL;
    size_t quantidade = (size_t)(g->n > 0 ? g->n : 1);
    size_t *grau = calloc(quantidade, sizeof *grau);
    int *ordem = malloc(quantidade * sizeof *ordem);
    if (!grau || !ordem) {
        free(grau);
        free(ordem);
        return NULL;
    }
    for (int v = 0; v < g->n; ++v)
        for (No *a = g->adj[v]; a; a = a->prox)
            ++grau[a->destino];

    /* O proprio array de saida funciona como fila FIFO. */
    int inicio = 0, fim = 0;
    for (int v = 0; v < g->n; ++v)
        if (grau[v] == 0) ordem[fim++] = v;
    while (inicio < fim) {
        int v = ordem[inicio++];
        for (No *a = g->adj[v]; a; a = a->prox)
            if (--grau[a->destino] == 0) ordem[fim++] = a->destino;
    }
    free(grau);
    if (fim != g->n) {
        free(ordem); /* Vertices restantes pertencem ou dependem de ciclos. */
        return NULL;
    }
    *tamanho = fim;
    return ordem;
}

/* 0 = nao visitado, 1 = na pilha de chamadas, 2 = finalizado. */
static int visitar(GrafoLista *g, int v, unsigned char *cor,
                   int *ordem, int *posicao)
{
    cor[v] = 1;
    for (No *a = g->adj[v]; a; a = a->prox) {
        int w = a->destino;
        if (cor[w] == 1) return 0; /* Aresta de retorno: ciclo. */
        if (cor[w] == 0 && !visitar(g, w, cor, ordem, posicao)) return 0;
    }
    cor[v] = 2;
    /* Empilha na saida da DFS, preenchendo do fim para o inicio. */
    if (ordem) ordem[--(*posicao)] = v;
    return 1;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho)
{
    if (!tamanho) return NULL;
    *tamanho = 0;
    if (!g || g->n < 0) return NULL;
    size_t quantidade = (size_t)(g->n > 0 ? g->n : 1);
    unsigned char *cor = calloc(quantidade, sizeof *cor);
    int *ordem = malloc(quantidade * sizeof *ordem);
    if (!cor || !ordem) {
        free(cor);
        free(ordem);
        return NULL;
    }
    int posicao = g->n;
    for (int v = 0; v < g->n; ++v) {
        if (cor[v] == 0 && !visitar(g, v, cor, ordem, &posicao)) {
            free(cor);
            free(ordem);
            return NULL;
        }
    }
    free(cor);
    *tamanho = g->n;
    return ordem;
}

int eh_dag(GrafoLista *g)
{
    if (!g || g->n < 0) return -1;
    unsigned char *cor = calloc((size_t)(g->n > 0 ? g->n : 1), sizeof *cor);
    if (!cor) return -1;
    for (int v = 0; v < g->n; ++v) {
        if (cor[v] == 0 && !visitar(g, v, cor, NULL, NULL)) {
            free(cor);
            return 0;
        }
    }
    free(cor);
    return 1;
}

#include <stdlib.h>

#include "grafo_lista.h"

static int vertice_valido(const GrafoLista *g, int vertice)
{
    return g != NULL && vertice >= 0 && vertice < g->n;
}

static No *criar_no(int destino)
{
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        return NULL;
    }

    novo->destino = destino;
    novo->prox = NULL;

    return novo;
}

static int contem_vertice(const No *lista, int destino)
{
    const No *atual = lista;

    while (atual != NULL) {
        if (atual->destino == destino) {
            return 1;
        }

        atual = atual->prox;
    }

    return 0;
}

static int remover_no(GrafoLista *g, int origem, int destino)
{
    No *atual = g->adj[origem];
    No *anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == destino) {
            if (anterior == NULL) {
                g->adj[origem] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            return 0;
        }

        anterior = atual;
        atual = atual->prox;
    }

    return -1;
}

GrafoLista *criar_grafo_lista(int n)
{
    if (n < 0) {
        return NULL;
    }

    GrafoLista *g = malloc(sizeof(GrafoLista));

    if (g == NULL) {
        return NULL;
    }

    g->n = n;
    g->adj = NULL;

    if (n == 0) {
        return g;
    }

    g->adj = calloc((size_t)n, sizeof(No *));

    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

int inserir_aresta_lista(GrafoLista *g, int origem, int destino)
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
    if (contem_vertice(g->adj[origem], destino)) {
        return -1;
    }

    No *novo_destino = criar_no(destino);

    if (novo_destino == NULL) {
        return -1;
    }

    novo_destino->prox = g->adj[origem];
    g->adj[origem] = novo_destino;

    /*
     * Como o grafo e nao direcionado,
     * adicionamos tambem a ligacao inversa.
     */
    No *nova_origem = criar_no(origem);

    if (nova_origem == NULL) {
        remover_no(g, origem, destino);
        return -1;
    }

    nova_origem->prox = g->adj[destino];
    g->adj[destino] = nova_origem;

    return 0;
}

int remover_aresta_lista(GrafoLista *g, int origem, int destino)
{
    if (!vertice_valido(g, origem) || !vertice_valido(g, destino)) {
        return -1;
    }

    if (!contem_vertice(g->adj[origem], destino)) {
        return -1;
    }

    remover_no(g, origem, destino);
    remover_no(g, destino, origem);

    return 0;
}

int grau_lista(const GrafoLista *g, int vertice)
{
    if (!vertice_valido(g, vertice)) {
        return -1;
    }

    int grau = 0;
    const No *atual = g->adj[vertice];

    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }

    return grau;
}

int sao_adjacentes_lista(const GrafoLista *g, int origem, int destino)
{
    if (!vertice_valido(g, origem) || !vertice_valido(g, destino)) {
        return -1;
    }

    return contem_vertice(g->adj[origem], destino);
}

void liberar_grafo_lista(GrafoLista *g)
{
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];

        while (atual != NULL) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }

    free(g->adj);
    free(g);
}
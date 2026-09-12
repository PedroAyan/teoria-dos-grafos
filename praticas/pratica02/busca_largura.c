#include "busca_largura.h"

#include <stdlib.h>

static int vertice_valido(const GrafoLista *g, int vertice)
{
    return g != NULL && vertice >= 0 && vertice < g->n;
}

static No *criar_no(int destino)
{
    No *novo = malloc(sizeof *novo);

    if (novo == NULL) {
        return NULL;
    }

    novo->destino = destino;
    novo->prox = NULL;
    return novo;
}

static int contem_destino(const No *lista, int destino)
{
    for (const No *atual = lista; atual != NULL; atual = atual->prox) {
        if (atual->destino == destino) {
            return 1;
        }
    }

    return 0;
}

static int fila_inicializar(Fila *fila, int capacidade)
{
    if (fila == NULL || capacidade <= 0) {
        return 0;
    }

    fila->dados = malloc((size_t)capacidade * sizeof *fila->dados);
    if (fila->dados == NULL) {
        return 0;
    }

    fila->capacidade = capacidade;
    fila->inicio = 0;
    fila->fim = 0;
    fila->tamanho = 0;
    return 1;
}

static void fila_liberar(Fila *fila)
{
    if (fila != NULL) {
        free(fila->dados);
        fila->dados = NULL;
        fila->capacidade = 0;
        fila->inicio = 0;
        fila->fim = 0;
        fila->tamanho = 0;
    }
}

static int fila_inserir(Fila *fila, int valor)
{
    if (fila == NULL || fila->tamanho == fila->capacidade) {
        return 0;
    }

    fila->dados[fila->fim] = valor;
    fila->fim = (fila->fim + 1) % fila->capacidade;
    fila->tamanho++;
    return 1;
}

static int fila_remover(Fila *fila, int *valor)
{
    if (fila == NULL || valor == NULL || fila->tamanho == 0) {
        return 0;
    }

    *valor = fila->dados[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;
    return 1;
}

GrafoLista *criar_grafo_lista(int n)
{
    if (n < 0) {
        return NULL;
    }

    GrafoLista *g = malloc(sizeof *g);
    if (g == NULL) {
        return NULL;
    }

    g->n = n;
    g->adj = calloc((size_t)(n > 0 ? n : 1), sizeof *g->adj);
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

int inserir_aresta_lista(GrafoLista *g, int origem, int destino)
{
    if (!vertice_valido(g, origem) || !vertice_valido(g, destino)
        || origem == destino || contem_destino(g->adj[origem], destino)) {
        return 0;
    }

    No *para_destino = criar_no(destino);
    No *para_origem = criar_no(origem);
    if (para_destino == NULL || para_origem == NULL) {
        free(para_destino);
        free(para_origem);
        return 0;
    }

    para_destino->prox = g->adj[origem];
    g->adj[origem] = para_destino;

    para_origem->prox = g->adj[destino];
    g->adj[destino] = para_origem;
    return 1;
}

void liberar_grafo_lista(GrafoLista *g)
{
    if (g == NULL) {
        return;
    }

    for (int v = 0; v < g->n; ++v) {
        No *atual = g->adj[v];
        while (atual != NULL) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }

    free(g->adj);
    free(g);
}

int bfs(GrafoLista *g, int origem, int *dist, int *pred)
{
    if (!vertice_valido(g, origem) || dist == NULL || pred == NULL) {
        return 0;
    }

    for (int v = 0; v < g->n; ++v) {
        dist[v] = -1;
        pred[v] = -1;
    }

    Fila fila;
    if (!fila_inicializar(&fila, g->n > 0 ? g->n : 1)) {
        return 0;
    }

    dist[origem] = 0;
    if (!fila_inserir(&fila, origem)) {
        fila_liberar(&fila);
        return 0;
    }

    int atual;
    while (fila_remover(&fila, &atual)) {
        for (No *vizinho = g->adj[atual]; vizinho != NULL;
             vizinho = vizinho->prox) {
            int destino = vizinho->destino;
            if (dist[destino] != -1) {
                continue;
            }

            dist[destino] = dist[atual] + 1;
            pred[destino] = atual;
            if (!fila_inserir(&fila, destino)) {
                fila_liberar(&fila);
                return 0;
            }
        }
    }

    fila_liberar(&fila);
    return 1;
}

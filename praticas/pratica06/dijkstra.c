#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

GrafoListaPonderado *criar_grafo_ponderado(int n)
{
    if (n <= 0) return NULL;
    GrafoListaPonderado *g = malloc(sizeof *g);
    if (!g) return NULL;
    g->adj = calloc((size_t)n, sizeof *g->adj);
    if (!g->adj) { free(g); return NULL; }
    g->n = n;
    return g;
}

int inserir_aresta_ponderada(GrafoListaPonderado *g, int origem,
                            int destino, int peso)
{
    if (!g || origem < 0 || origem >= g->n || destino < 0 ||
        destino >= g->n || peso < 0 || peso == INT_MAX) return -1;
    ArestaPonderada *ida = malloc(sizeof *ida);
    ArestaPonderada *volta = malloc(sizeof *volta);
    if (!ida || !volta) { free(ida); free(volta); return -1; }
    *ida = (ArestaPonderada){destino, peso, g->adj[origem]};
    g->adj[origem] = ida;
    *volta = (ArestaPonderada){origem, peso, g->adj[destino]};
    g->adj[destino] = volta;
    return 0;
}

void liberar_grafo_ponderado(GrafoListaPonderado *g)
{
    if (!g) return;
    for (int v = 0; v < g->n; ++v) {
        ArestaPonderada *a = g->adj[v];
        while (a) {
            ArestaPonderada *proxima = a->prox;
            free(a);
            a = proxima;
        }
    }
    free(g->adj);
    free(g);
}

MinHeap *criar_min_heap(int capacidade)
{
    if (capacidade <= 0) return NULL;
    MinHeap *heap = malloc(sizeof *heap);
    if (!heap) return NULL;
    heap->dados = malloc((size_t)capacidade * sizeof *heap->dados);
    heap->posicao = malloc((size_t)capacidade * sizeof *heap->posicao);
    if (!heap->dados || !heap->posicao) {
        liberar_min_heap(heap);
        return NULL;
    }
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    for (int v = 0; v < capacidade; ++v) heap->posicao[v] = -1;
    return heap;
}

/* Toda troca atualiza o mapeamento, sem procurar vertices no vetor. */
static void trocar(MinHeap *heap, int i, int j)
{
    NoHeap temporario = heap->dados[i];
    heap->dados[i] = heap->dados[j];
    heap->dados[j] = temporario;
    heap->posicao[heap->dados[i].vertice] = i;
    heap->posicao[heap->dados[j].vertice] = j;
}

static void subir(MinHeap *heap, int i)
{
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap->dados[pai].distancia <= heap->dados[i].distancia) break;
        trocar(heap, pai, i);
        i = pai;
    }
}

int inserir_heap(MinHeap *heap, int vertice, int distancia)
{
    if (!heap || vertice < 0 || vertice >= heap->capacidade ||
        heap->tamanho >= heap->capacidade || heap->posicao[vertice] != -1 ||
        distancia < 0) return -1;
    int i = heap->tamanho++;
    heap->dados[i] = (NoHeap){vertice, distancia};
    heap->posicao[vertice] = i;
    subir(heap, i);
    return 0;
}

NoHeap extrair_minimo(MinHeap *heap)
{
    if (!heap || heap->tamanho == 0) return (NoHeap){-1, INT_MAX};
    NoHeap minimo = heap->dados[0];
    --heap->tamanho;
    heap->posicao[minimo.vertice] = -1;
    if (heap->tamanho == 0) return minimo;
    heap->dados[0] = heap->dados[heap->tamanho];
    heap->posicao[heap->dados[0].vertice] = 0;
    int i = 0;
    /* i < tamanho/2 garante que existe filho esquerdo e evita overflow. */
    while (i < heap->tamanho / 2) {
        int esquerdo = 2 * i + 1;
        int direito = esquerdo + 1;
        int menor = esquerdo;
        if (direito < heap->tamanho &&
            heap->dados[direito].distancia < heap->dados[esquerdo].distancia)
            menor = direito;
        if (heap->dados[i].distancia <= heap->dados[menor].distancia) break;
        trocar(heap, i, menor);
        i = menor;
    }
    return minimo;
}

int diminuir_chave(MinHeap *heap, int vertice, int nova_distancia)
{
    if (!heap || vertice < 0 || vertice >= heap->capacidade ||
        nova_distancia < 0) return -1;
    int i = heap->posicao[vertice];
    if (i < 0 || nova_distancia > heap->dados[i].distancia) return -1;
    heap->dados[i].distancia = nova_distancia;
    subir(heap, i);
    return 0;
}

void liberar_min_heap(MinHeap *heap)
{
    if (!heap) return;
    free(heap->dados);
    free(heap->posicao);
    free(heap);
}

int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred)
{
    if (!g || !dist || !pred || origem < 0 || origem >= g->n) return -1;
    for (int v = 0; v < g->n; ++v) {
        dist[v] = INT_MAX;
        pred[v] = -1;
    }
    /* Valida tambem grafos montados diretamente pelo chamador. */
    for (int v = 0; v < g->n; ++v)
        for (ArestaPonderada *a = g->adj[v]; a; a = a->prox)
            if (a->peso < 0 || a->peso == INT_MAX ||
                a->destino < 0 || a->destino >= g->n) return -1;

    MinHeap *heap = criar_min_heap(g->n);
    if (!heap) return -1;
    for (int v = 0; v < g->n; ++v) inserir_heap(heap, v, INT_MAX);
    dist[origem] = 0;
    diminuir_chave(heap, origem, 0);

    while (heap->tamanho > 0) {
        NoHeap minimo = extrair_minimo(heap);
        int u = minimo.vertice;
        if (dist[u] == INT_MAX) break; /* Restam apenas vertices inalcançaveis. */
        for (ArestaPonderada *a = g->adj[u]; a; a = a->prox) {
            int v = a->destino;
            if (heap->posicao[v] == -1) continue; /* Ja finalizado. */
            /* A soma so e feita quando cabe em int. */
            if (a->peso > INT_MAX - dist[u]) continue;
            int alternativa = dist[u] + a->peso;
            if (alternativa < dist[v]) {
                dist[v] = alternativa;
                pred[v] = u;
                diminuir_chave(heap, v, alternativa);
            }
        }
    }
    liberar_min_heap(heap);
    return 0;
}

static void imprimir_sequencia(int origem, int destino, const int *pred)
{
    if (destino == origem) { printf("%d", origem); return; }
    imprimir_sequencia(origem, pred[destino], pred);
    printf(" -> %d", destino);
}

void reconstruir_caminho(int origem, int destino, int *pred)
{
    if (!pred || origem < 0 || destino < 0) {
        printf("Parametros invalidos\n");
        return;
    }
    if (destino != origem && pred[destino] == -1) {
        printf("Sem caminho de %d para %d\n", origem, destino);
        return;
    }
    imprimir_sequencia(origem, destino, pred);
    putchar('\n');
}

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

/* Grafo ponderado nao direcionado; vertices de 0 a n-1.
 * INT_MAX representa infinito e nao pode ser um custo finito. */
typedef struct ArestaPonderada {
    int destino;
    int peso;
    struct ArestaPonderada *prox;
} ArestaPonderada;

typedef struct {
    int n;
    ArestaPonderada **adj;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; /* vertice -> indice; -1 quando fora do heap */
} MinHeap;

GrafoListaPonderado *criar_grafo_ponderado(int n);
/* Retorna 0 no sucesso e -1 para erro ou peso invalido. */
int inserir_aresta_ponderada(GrafoListaPonderado *g, int origem,
                            int destino, int peso);
void liberar_grafo_ponderado(GrafoListaPonderado *g);

MinHeap *criar_min_heap(int capacidade);
int inserir_heap(MinHeap *heap, int vertice, int distancia);
/* Heap vazio: retorna { -1, INT_MAX }. */
NoHeap extrair_minimo(MinHeap *heap);
/* Retorna 0 no sucesso e -1 para chave/vertice invalido. */
int diminuir_chave(MinHeap *heap, int vertice, int nova_distancia);
void liberar_min_heap(MinHeap *heap);

/* dist e pred devem ter g->n posicoes. Retorna 0 no sucesso, -1 no erro.
 * Sem caminho: dist[v] = INT_MAX e pred[v] = -1.
 * Custos finitos devem ser menores que INT_MAX. */
int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
/* Imprime o caminho usando pred produzido por dijkstra para essa origem.
 * origem e destino devem ser indices validos desse vetor. */
void reconstruir_caminho(int origem, int destino, int *pred);

#endif

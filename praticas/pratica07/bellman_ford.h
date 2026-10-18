#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

/* Grafo direcionado ponderado, com vertices de 0 a n-1. */
typedef struct ArestaPonderada {
    int destino;
    int peso;
    struct ArestaPonderada *prox;
} ArestaPonderada;

typedef struct {
    int n;
    ArestaPonderada **adj;
    int ultimo_relaxado; /* testemunha da n-esima rodada; -1 sem ciclo */
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_ponderado(int n);
/* Insere somente origem -> destino. Retorna 0 no sucesso, -1 no erro. */
int inserir_aresta_ponderada(GrafoListaPonderado *g, int origem,
                            int destino, int peso);
void liberar_grafo_ponderado(GrafoListaPonderado *g);

/* dist e pred devem ter n posicoes.
 * Retorna 1 no sucesso e 0 se houver ciclo negativo ALCANCAVEL da origem.
 * Retorna -1 para entrada invalida, falta de memoria ou custo fora da faixa.
 * No sucesso: INT_MAX significa inalcançavel e pred = -1 sem predecessor.
 * Custos finitos devem estar entre INT_MIN e INT_MAX-1.
 * Com ciclo negativo, dist nao representa caminhos minimos; use pred e
 * g->ultimo_relaxado para reconstruir o ciclo. */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Retrocede n vezes em pred, imprime um ciclo fechado na direcao das
 * arestas e retorna 1. Retorna 0 em caso de erro/ausencia de ciclo.
 * Use n = g->n e vertice = g->ultimo_relaxado apos bellman_ford retornar 0. */
int reconstruir_ciclo_negativo(int n, int vertice, const int *pred);

#endif

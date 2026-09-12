#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

/* Grafo nao direcionado representado por listas de adjacencia. */
typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

/* Fila FIFO usada pela BFS (implementada como array circular). */
typedef struct {
    int *dados;
    int capacidade;
    int inicio;
    int fim;
    int tamanho;
} Fila;

GrafoLista *criar_grafo_lista(int n);
int inserir_aresta_lista(GrafoLista *g, int origem, int destino);
void liberar_grafo_lista(GrafoLista *g);

/*
 * Executa BFS a partir de origem.
 * Ao final, dist[v] e a distancia de origem a v, ou -1 se v nao for alcancado;
 * pred[v] e o predecessor de v na arvore BFS, ou -1 para a origem/inalcancavel.
 * Retorna 1 em caso de sucesso e 0 se os argumentos forem invalidos.
 */
int bfs(GrafoLista *g, int origem, int *dist, int *pred);

#endif

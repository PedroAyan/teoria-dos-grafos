#ifndef DAG_H
#define DAG_H

/* Mesma representacao por lista de adjacencia da Pratica 01. */
typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

GrafoLista *criar_grafo_lista(int n);
int inserir_aresta_lista(GrafoLista *g, int origem, int destino);
void liberar_grafo_lista(GrafoLista *g);

/* Vertices de 0 a n-1. O chamador deve liberar o array retornado.
 * Ciclo ou erro: NULL e *tamanho = 0. Grafo vazio: array e tamanho 0.
 * g deve ser um grafo valido; tamanho deve ser diferente de NULL. */
int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);
int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);
/* Retorna 1 para DAG, 0 para ciclo e -1 para erro. */
int eh_dag(GrafoLista *g);

#endif
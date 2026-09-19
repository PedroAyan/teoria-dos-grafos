#ifndef COLORACAO_H
#define COLORACAO_H

/* Grafo simples nao direcionado. Vertices numerados de 0 a n-1. */
typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

GrafoLista *criar_grafo_lista(int n);
/* 0 no sucesso; -1 para erro, laco ou aresta repetida. */
int inserir_aresta_lista(GrafoLista *g, int origem, int destino);
void liberar_grafo_lista(GrafoLista *g);

/* Retornam cor[v], com cores a partir de zero. Liberar o array com free.
 * NULL indica erro; num_cores fica em zero se fornecido.
 * Grafo vazio retorna um array liberavel e zero cores.
 * As heuristicas produzem coloracoes validas, mas nao garantem o minimo. */
int *coloracao_gulosa(GrafoLista *g, int *num_cores);
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores);

/* BFS em todas as componentes: 1 bipartido, 0 nao bipartido, -1 erro.
 * Bipartido significa colorivel com NO MAXIMO duas cores.
 * Um grafo sem arestas pode precisar de apenas uma cor (zero se vazio). */
int eh_bipartido(GrafoLista *g);

#endif

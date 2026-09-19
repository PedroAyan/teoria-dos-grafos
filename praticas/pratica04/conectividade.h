#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

/* Grafos simples, nao direcionados, com vertices de 0 a n-1. */
typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

typedef struct { int origem, destino; } Ponte;

GrafoLista *criar_grafo_lista(int n);
/* Retorna 0 no sucesso e -1 para erro, laco ou aresta repetida. */
int inserir_aresta_lista(GrafoLista *g, int origem, int destino);
void liberar_grafo_lista(GrafoLista *g);

/* Arrays de n elementos. Inicializar descoberta/low/articulacao e tempo
 * com zero; chamar para cada raiz ainda nao descoberta, com pai = -1.
 * Se pontes != NULL, reservar n elementos e iniciar quantidade em zero. */
void dfs_articulacoes(const GrafoLista *g, int u, int pai, int *tempo,
                     int descoberta[], int low[], int articulacao[],
                     Ponte pontes[], int *quantidade);
/* Percorre inclusive componentes desconexos. Retorna quantidade ou -1.
 * O chamador fornece articulacao[n] e pontes[n], respectivamente. */
int detectar_articulacoes(const GrafoLista *g, int articulacao[]);
int detectar_pontes(const GrafoLista *g, Ponte pontes[]);

#endif

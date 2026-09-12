#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "busca_largura.h"

/* Pilha LIFO usada pela DFS iterativa (estrutura auxiliar da pratica). */
typedef struct {
    int *dados;
    int topo;
    int capacidade;
} Pilha;

/*
 * DFS recursiva a partir de u.
 * visitado deve ser um vetor de g->n inteiros inicializado com zero.
 * tempo, entrada e saida permitem registrar os tempos de entrada/saida;
 * os tres devem ser informados juntos para obter a temporizacao completa.
 */
void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo,
                   int *entrada, int *saida);

int contar_componentes(GrafoLista *g);
int tem_ciclo(GrafoLista *g);
int eh_bipartido(GrafoLista *g);

#endif

#ifndef TPS_H
#define TPS_H

#include <stdbool.h>

/* Matrizes n x n em ordem de linhas. Os algoritmos recebem n > 0,
 * pesos simetricos nao negativos com diagonal zero e uma rota de
 * n+1 posicoes. Retornam o custo, com a rota fechada em 0, ou -1 em erro. */
void grafo_para_tsp(int n, const bool *adj, int *pesos);
long long tsp_forca_bruta(int n, const int *pesos, int *rota);
long long tsp_vizinho_mais_proximo(int n, const int *pesos, int *rota);
long long tsp_insercao(int n, const int *pesos, int *rota);

#endif

#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

#include <stdbool.h>

/* Matriz de adjacencia n x n em ordem de linhas. O caminho usa n
 * posicoes e o ciclo usa n+1, com a origem repetida no final.
 * Em um grafo simples, um ciclo exige pelo menos tres vertices. */
bool encontrar_caminho_hamiltoniano_bt(int n, const bool *adj, int *caminho);
bool encontrar_ciclo_hamiltoniano_bt(int n, const bool *adj, int *ciclo);

#endif

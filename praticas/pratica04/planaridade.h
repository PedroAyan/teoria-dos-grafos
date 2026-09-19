#ifndef PLANARIDADE_H
#define PLANARIDADE_H
#include "conectividade.h"

/* Euler: 1 satisfaz a condicao NECESSARIA m <= 3n-6; 0 nao planar;
 * -1 entrada invalida. Para n < 3, todo grafo simples e planar. */
int eh_planar_euler(GrafoLista *g);
/* Busca exata de subdivisoes: 1 encontrou K5 ou K3,3, 0 nao encontrou;
 * -1 entrada invalida ou n > 10 (fora do limite da forca bruta). */
int contem_subdivisao_kuratowski(GrafoLista *g);
/* 1 planar, 0 nao planar, 2 inconclusivo (n > 10 e passou Euler),
 * -1 entrada invalida. Euler sozinho nunca confirma planaridade. */
int eh_planar(GrafoLista *g);
#endif

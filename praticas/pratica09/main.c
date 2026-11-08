#include "cobertura.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { MAX_VERTICES = 15, REPETICOES = 10000 };

static int gerar_grafo(int n, Aresta *arestas)
{
    uint32_t estado = (uint32_t)n;
    int m = 0;
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            estado = estado * UINT32_C(1664525) + UINT32_C(1013904223);
            if ((estado >> 16) % 100 < 40)
                arestas[m++] = (Aresta){u, v};
        }
    }
    return m;
}

typedef int (*Algoritmo)(int, const Aresta *, int, bool *);

static int medir(Algoritmo algoritmo, int n, const Aresta *arestas, int m,
                 double *microssegundos)
{
    bool selecao[MAX_VERTICES];
    int tamanho = 0;
    clock_t inicio = clock();
    for (int i = 0; i < REPETICOES; ++i) {
        tamanho = algoritmo(n, arestas, m, selecao);
        if (tamanho < 0)
            return -1;
    }
    clock_t fim = clock();
    if (inicio == (clock_t)-1 || fim == (clock_t)-1)
        return -1;

    int contagem = 0;
    for (int v = 0; v < n; ++v)
        contagem += selecao[v];
    if (contagem != tamanho || !eh_cobertura(arestas, m, selecao))
        return -1;

    *microssegundos = (double)(fim - inicio) * 1e6 / CLOCKS_PER_SEC / REPETICOES;
    return tamanho;
}

int main(void)
{
    const int tamanhos[] = {5, 8, 10, 12, 15};
    Aresta arestas[MAX_VERTICES * (MAX_VERTICES - 1) / 2];

    printf("Tempo medio por chamada (us), %d repeticoes por algoritmo\n", REPETICOES);
    printf("%3s %7s %7s %7s %12s %12s\n", "n", "arestas", "minimo",
           "gulosa", "backtracking", "gulosa (us)");
    for (size_t i = 0; i < sizeof tamanhos / sizeof tamanhos[0]; ++i) {
        int n = tamanhos[i];
        int m = gerar_grafo(n, arestas);
        double tempo_bt, tempo_gulosa;
        int minimo = medir(cobertura_minima_bt, n, arestas, m, &tempo_bt);
        int gulosa = medir(cobertura_gulosa, n, arestas, m, &tempo_gulosa);
        if (minimo < 0 || gulosa < minimo) {
            fprintf(stderr, "Falha no experimento para n=%d\n", n);
            return EXIT_FAILURE;
        }

        printf("%3d %7d %7d %7d %12.3f %12.3f\n", n, m, minimo, gulosa,
               tempo_bt, tempo_gulosa);
    }
    return EXIT_SUCCESS;
}

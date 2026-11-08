#include "tps.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int peso(int n, const int *pesos, int u, int v)
{
    return pesos[(size_t)u * n + v];
}

static void trocar(int *a, int *b)
{
    int valor = *a;
    *a = *b;
    *b = valor;
}

void grafo_para_tsp(int n, const bool *adj, int *pesos)
{
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            size_t i = (size_t)u * n + v;
            pesos[i] = u == v ? 0 : (adj[i] ? 1 : 2);
        }
    }
}

static void permutar(int n, const int *pesos, int *atual, int pos,
                     long long custo, long long *melhor, int *rota)
{
    int anterior = atual[pos - 1];
    if (pos == n) {
        custo += peso(n, pesos, anterior, 0);
        if (custo < *melhor) {
            *melhor = custo;
            memcpy(rota, atual, (size_t)n * sizeof *rota);
        }
        return;
    }

    for (int i = pos; i < n; ++i) {
        trocar(&atual[pos], &atual[i]);
        permutar(n, pesos, atual, pos + 1,
                 custo + peso(n, pesos, anterior, atual[pos]), melhor, rota);
        trocar(&atual[pos], &atual[i]);
    }
}

long long tsp_forca_bruta(int n, const int *pesos, int *rota)
{
    if (n < 1)
        return -1;

    int *atual = calloc((size_t)n, sizeof *atual);
    if (!atual)
        return -1;
    for (int v = 0; v < n; ++v)
        atual[v] = v;

    /* Origem fixa, sem poda: visita as (n-1)! ordens restantes. */
    long long melhor = LLONG_MAX;
    permutar(n, pesos, atual, 1, 0, &melhor, rota);
    rota[n] = 0;
    free(atual);
    return melhor;
}

long long tsp_vizinho_mais_proximo(int n, const int *pesos, int *rota)
{
    if (n < 1)
        return -1;

    for (int v = 0; v < n; ++v)
        rota[v] = v;

    long long custo = 0;
    for (int pos = 1; pos < n; ++pos) {
        int anterior = rota[pos - 1];
        int mais_proximo = pos;
        for (int i = pos + 1; i < n; ++i) {
            if (peso(n, pesos, anterior, rota[i]) <
                peso(n, pesos, anterior, rota[mais_proximo]))
                mais_proximo = i;
        }
        trocar(&rota[pos], &rota[mais_proximo]);
        custo += peso(n, pesos, anterior, rota[pos]);
    }
    rota[n] = 0;
    return custo + peso(n, pesos, rota[n - 1], 0);
}

long long tsp_insercao(int n, const int *pesos, int *rota)
{
    if (n < 1)
        return -1;

    for (int v = 0; v < n; ++v)
        rota[v] = v;

    long long custo = 0;
    for (int usados = 1; usados < n; ++usados) {
        /* Prefixo: ciclo parcial. Sufixo: vertices ainda fora do ciclo. */
        int escolhido = usados;
        int distancia = peso(n, pesos, rota[0], rota[usados]);
        for (int i = usados; i < n; ++i) {
            for (int j = 0; j < usados; ++j) {
                int d = peso(n, pesos, rota[j], rota[i]);
                if (d < distancia) {
                    distancia = d;
                    escolhido = i;
                }
            }
        }

        int v = rota[escolhido];
        int pos = 0;
        long long aumento = LLONG_MAX;
        for (int j = 0; j < usados; ++j) {
            int a = rota[j];
            int b = rota[(j + 1) % usados];
            long long delta = (long long)peso(n, pesos, a, v) +
                              peso(n, pesos, v, b) - peso(n, pesos, a, b);
            if (delta < aumento) {
                aumento = delta;
                pos = j + 1;
            }
        }

        trocar(&rota[escolhido], &rota[usados]);
        memmove(&rota[pos + 1], &rota[pos], (size_t)(usados - pos) * sizeof *rota);
        rota[pos] = v;
        custo += aumento;
    }
    rota[n] = 0;
    return custo;
}

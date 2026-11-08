#include "hamiltoniano.h"
#include "tps.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { MAX_VERTICES = 12, REPETICOES = 200000 };

typedef bool (*Busca)(int, const bool *, int *);
typedef long long (*AlgoritmoTsp)(int, const int *, int *);

static void gerar_grafo(int n, bool *adj)
{
    uint32_t estado = (uint32_t)n;
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            estado = estado * UINT32_C(1664525) + UINT32_C(1013904223);
            bool aresta = (estado >> 16) % 100 < 45;

            /* Casos de controle: anel, caminho e dois componentes. */
            if (n == 5)
                aresta = v == u + 1 || (u == 0 && v == n - 1);
            if (n == 8)
                aresta = v == u + 1;
            if (n == 10)
                aresta = (u < n / 2) == (v < n / 2);
            adj[u * n + v] = adj[v * n + u] = aresta;
        }
    }
}

static bool permutacao(int n, const int *rota)
{
    bool vistos[MAX_VERTICES] = {false};
    for (int i = 0; i < n; ++i) {
        int v = rota[i];
        if (v < 0 || v >= n || vistos[v])
            return false;
        vistos[v] = true;
    }
    return true;
}

static bool medir_busca(Busca busca, int n, const bool *adj, bool ciclo,
                        bool *encontrou, double *tempo_us)
{
    int rota[MAX_VERTICES + 1];
    clock_t inicio = clock();
    for (int i = 0; i < REPETICOES; ++i)
        *encontrou = busca(n, adj, rota);
    clock_t fim = clock();
    if (inicio == (clock_t)-1 || fim == (clock_t)-1)
        return false;
    *tempo_us = (double)(fim - inicio) * 1e6 / CLOCKS_PER_SEC / REPETICOES;

    if (!*encontrou)
        return true;
    if (!permutacao(n, rota) || (ciclo && rota[n] != rota[0]))
        return false;
    for (int i = 1; i < n + ciclo; ++i) {
        if (!adj[rota[i - 1] * n + rota[i]])
            return false;
    }
    return true;
}

static long long medir_tsp(AlgoritmoTsp algoritmo, int n, const int *pesos,
                           int repeticoes, double *tempo_us)
{
    int rota[MAX_VERTICES + 1];
    long long custo = -1;
    clock_t inicio = clock();
    for (int i = 0; i < repeticoes; ++i) {
        custo = algoritmo(n, pesos, rota);
        if (custo < 0)
            return -1;
    }
    clock_t fim = clock();
    if (inicio == (clock_t)-1 || fim == (clock_t)-1)
        return -1;
    *tempo_us = (double)(fim - inicio) * 1e6 / CLOCKS_PER_SEC / repeticoes;

    if (!permutacao(n, rota) || rota[0] != 0 || rota[n] != 0)
        return -1;
    long long soma = 0;
    for (int i = 1; i <= n; ++i)
        soma += pesos[rota[i - 1] * n + rota[i]];
    return soma == custo ? custo : -1;
}

static bool experimentar(int n, int repeticoes_exato)
{
    bool adj[MAX_VERTICES * MAX_VERTICES] = {false};
    int pesos[MAX_VERTICES * MAX_VERTICES];
    gerar_grafo(n, adj);
    grafo_para_tsp(n, adj, pesos);

    bool caminho, ciclo;
    double tempo_caminho, tempo_ciclo;
    if (!medir_busca(encontrar_caminho_hamiltoniano_bt, n, adj, false,
                     &caminho, &tempo_caminho) ||
        !medir_busca(encontrar_ciclo_hamiltoniano_bt, n, adj, true,
                     &ciclo, &tempo_ciclo))
        return false;

    double tempo_exato, tempo_vizinho, tempo_insercao;
    long long exato = medir_tsp(tsp_forca_bruta, n, pesos, repeticoes_exato,
                                &tempo_exato);
    long long vizinho = medir_tsp(tsp_vizinho_mais_proximo, n, pesos, REPETICOES,
                                  &tempo_vizinho);
    long long insercao = medir_tsp(tsp_insercao, n, pesos, REPETICOES, &tempo_insercao);

    if (exato < n || vizinho < exato || insercao < exato || ciclo != (exato == n) ||
        (ciclo && !caminho) || (n == 5 && exato != 5) ||
        (n == 8 && (!caminho || exato != 9)) || (n == 10 && (caminho || exato != 12)))
        return false;

    printf("%2d | %3s %10.3f | %3s %10.3f | %5lld %12.3f | %5lld %9.3f | %5lld %9.3f\n",
           n, caminho ? "sim" : "nao", tempo_caminho, ciclo ? "sim" : "nao", tempo_ciclo,
           exato, tempo_exato, vizinho, tempo_vizinho, insercao, tempo_insercao);
    return true;
}

int main(void)
{
    const int tamanhos[] = {5, 8, 10, 12};
    const int repeticoes_exato[] = {10000, 1000, 10, 1};
    puts("Tempo medio em microssegundos (us) por chamada");
    puts(" n | caminho (us)   | ciclo (us)     | TSP exato (us)     | vizinho (us)    | insercao (us)");
    for (size_t i = 0; i < sizeof tamanhos / sizeof tamanhos[0]; ++i) {
        if (!experimentar(tamanhos[i], repeticoes_exato[i])) {
            fprintf(stderr, "Falha na validacao para n=%d\n", tamanhos[i]);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

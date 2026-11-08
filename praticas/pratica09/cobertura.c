#include "cobertura.h"
#include <stdlib.h>
#include <string.h>

bool eh_cobertura(const Aresta *arestas, int m, const bool *selecao)
{
    for (int i = 0; i < m; ++i) {
        if (!selecao[arestas[i].u] && !selecao[arestas[i].v])
            return false;
    }
    return true;
}

static void buscar(const Aresta *arestas, int m, int inicio, int n,
                   bool *atual, int tamanho, bool *melhor, int *limite)
{
    if (tamanho >= *limite)
        return;

    int i = inicio;
    while (i < m && (atual[arestas[i].u] || atual[arestas[i].v]))
        ++i;

    if (i == m) {
        memcpy(melhor, atual, (size_t)n * sizeof *melhor);
        *limite = tamanho;
        return;
    }

    /* Toda cobertura precisa conter um extremo desta aresta descoberta. */
    int u = arestas[i].u;
    int v = arestas[i].v;
    atual[u] = true;
    buscar(arestas, m, i + 1, n, atual, tamanho + 1, melhor, limite);
    atual[u] = false;

    if (v != u) {
        atual[v] = true;
        buscar(arestas, m, i + 1, n, atual, tamanho + 1, melhor, limite);
        atual[v] = false;
    }
}

int cobertura_minima_bt(int n, const Aresta *arestas, int m, bool *selecao)
{
    if (m == 0) {
        for (int v = 0; v < n; ++v)
            selecao[v] = false;
        return 0;
    }

    bool *atual = calloc((size_t)n, sizeof *atual);
    if (!atual)
        return -1;

    /* Selecionar todos os vertices fornece o limite superior inicial. */
    for (int v = 0; v < n; ++v)
        selecao[v] = true;
    int limite = n;
    buscar(arestas, m, 0, n, atual, 0, selecao, &limite);
    free(atual);
    return limite;
}

int cobertura_gulosa(int n, const Aresta *arestas, int m, bool *selecao)
{
    for (int v = 0; v < n; ++v)
        selecao[v] = false;
    if (m == 0)
        return 0;

    int *graus = calloc((size_t)n, sizeof *graus);
    if (!graus)
        return -1;

    int tamanho = 0;
    for (;;) {
        memset(graus, 0, (size_t)n * sizeof *graus);
        for (int i = 0; i < m; ++i) {
            int u = arestas[i].u;
            int v = arestas[i].v;
            if (!selecao[u] && !selecao[v]) {
                ++graus[u];
                if (v != u)
                    ++graus[v];
            }
        }

        int maior = 0;
        for (int v = 1; v < n; ++v) {
            if (graus[v] > graus[maior])
                maior = v;
        }
        if (graus[maior] == 0)
            break;
        selecao[maior] = true;
        ++tamanho;
    }

    free(graus);
    return tamanho;
}

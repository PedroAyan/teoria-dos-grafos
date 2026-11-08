#include "hamiltoniano.h"
#include <stddef.h>

static void trocar(int *a, int *b)
{
    int valor = *a;
    *a = *b;
    *b = valor;
}

static bool buscar(int n, const bool *adj, int *rota, int pos, bool ciclo)
{
    if (pos == n)
        return !ciclo || adj[(size_t)rota[n - 1] * n + rota[0]];

    for (int i = pos; i < n; ++i) {
        if (pos > 0 && !adj[(size_t)rota[pos - 1] * n + rota[i]])
            continue;

        trocar(&rota[pos], &rota[i]);
        if (buscar(n, adj, rota, pos + 1, ciclo))
            return true;
        trocar(&rota[pos], &rota[i]);
    }
    return false;
}

bool encontrar_caminho_hamiltoniano_bt(int n, const bool *adj, int *caminho)
{
    if (n < 1)
        return false;

    for (int v = 0; v < n; ++v)
        caminho[v] = v;
    return buscar(n, adj, caminho, 0, false);
}

bool encontrar_ciclo_hamiltoniano_bt(int n, const bool *adj, int *ciclo)
{
    if (n < 3)
        return false;

    for (int v = 0; v < n; ++v)
        ciclo[v] = v;

    /* Qualquer ciclo pode ser rotacionado para comecar no vertice 0. */
    if (!buscar(n, adj, ciclo, 1, true))
        return false;
    ciclo[n] = ciclo[0];
    return true;
}

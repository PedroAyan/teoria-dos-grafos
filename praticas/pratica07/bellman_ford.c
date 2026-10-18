#include "bellman_ford.h"
#include <stdio.h>
#include <stdlib.h>

GrafoListaPonderado *criar_grafo_ponderado(int n)
{
    if (n <= 0) return NULL;
    GrafoListaPonderado *g = malloc(sizeof *g);
    if (!g) return NULL;
    g->adj = calloc((size_t)n, sizeof *g->adj);
    if (!g->adj) { free(g); return NULL; }
    g->n = n;
    g->ultimo_relaxado = -1;
    return g;
}

int inserir_aresta_ponderada(GrafoListaPonderado *g, int origem,
                            int destino, int peso)
{
    if (!g || origem < 0 || origem >= g->n ||
        destino < 0 || destino >= g->n) return -1;
    ArestaPonderada *a = malloc(sizeof *a);
    if (!a) return -1;
    *a = (ArestaPonderada){destino, peso, g->adj[origem]};
    g->adj[origem] = a;
    return 0;
}

void liberar_grafo_ponderado(GrafoListaPonderado *g)
{
    if (!g) return;
    for (int v = 0; v < g->n; ++v) {
        ArestaPonderada *a = g->adj[v];
        while (a) {
            ArestaPonderada *proxima = a->prox;
            free(a);
            a = proxima;
        }
    }
    free(g->adj);
    free(g);
}

/* Uma rodada percorre todas as arestas e guarda a ultima melhora.
 * long long evita overflow de int durante a deteccao de ciclos. */
static int relaxar_arestas(const GrafoListaPonderado *g, long long *custos,
                          int *pred, int *ultimo)
{
    *ultimo = -1;
    for (int u = 0; u < g->n; ++u) {
        if (custos[u] == LLONG_MAX) continue;
        for (ArestaPonderada *a = g->adj[u]; a; a = a->prox) {
            if ((a->peso > 0 && custos[u] >= LLONG_MAX - a->peso) ||
                (a->peso < 0 && custos[u] < LLONG_MIN - a->peso)) return -1;
            long long alternativa = custos[u] + a->peso;
            if (alternativa < custos[a->destino]) {
                custos[a->destino] = alternativa;
                pred[a->destino] = u;
                *ultimo = a->destino;
            }
        }
    }
    return 0;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred)
{
    if (!g || !g->adj || g->n <= 0 || !dist || !pred ||
        origem < 0 || origem >= g->n) return -1;
    g->ultimo_relaxado = -1;
    for (int v = 0; v < g->n; ++v) {
        dist[v] = INT_MAX;
        pred[v] = -1;
        for (ArestaPonderada *a = g->adj[v]; a; a = a->prox)
            if (a->destino < 0 || a->destino >= g->n) return -1;
    }
    long long *custos = malloc((size_t)g->n * sizeof *custos);
    if (!custos) return -1;
    for (int v = 0; v < g->n; ++v) custos[v] = LLONG_MAX;
    custos[origem] = 0;

    int ultimo = -1;
    int resultado = -1;
    /* As n-1 rodadas exigidas pelo enunciado. */
    for (int rodada = 0; rodada < g->n - 1; ++rodada)
        if (relaxar_arestas(g, custos, pred, &ultimo) != 0) goto fim;

    /* Melhora na n-esima rodada implica ciclo negativo alcancavel.
     * Atualizar pred nesta rodada permite reconstruir esse ciclo. */
    if (relaxar_arestas(g, custos, pred, &ultimo) != 0) goto fim;
    if (ultimo != -1) {
        g->ultimo_relaxado = ultimo;
        resultado = 0;
        goto fim;
    }
    for (int v = 0; v < g->n; ++v)
        if (custos[v] != LLONG_MAX &&
            (custos[v] < INT_MIN || custos[v] >= INT_MAX)) goto fim;
    for (int v = 0; v < g->n; ++v)
        if (custos[v] != LLONG_MAX) dist[v] = (int)custos[v];
    resultado = 1;
fim:
    free(custos);
    return resultado;
}

int reconstruir_ciclo_negativo(int n, int vertice, const int *pred)
{
    if (n <= 0 || !pred || vertice < 0 || vertice >= n) return 0;
    /* A testemunha pode estar depois do ciclo. Retroceder n vezes
     * garante que o vertice escolhido esta dentro dele. */
    for (int i = 0; i < n; ++i) {
        vertice = pred[vertice];
        if (vertice < 0 || vertice >= n) return 0;
    }
    int *ciclo = malloc((size_t)n * sizeof *ciclo);
    if (!ciclo) return 0;
    int atual = vertice;
    int tamanho = 0;
    do {
        if (atual < 0 || atual >= n || tamanho >= n) {
            free(ciclo);
            return 0;
        }
        ciclo[tamanho++] = atual;
        atual = pred[atual];
    } while (atual != vertice);

    /* pred percorre as arestas ao contrario; imprimir em ordem inversa. */
    printf("Ciclo negativo: %d", ciclo[tamanho - 1]);
    for (int i = tamanho - 2; i >= 0; --i) printf(" -> %d", ciclo[i]);
    printf(" -> %d\n", ciclo[tamanho - 1]);
    free(ciclo);
    return 1;
}

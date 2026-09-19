#include "coloracao.h"
#include <stdlib.h>

GrafoLista *criar_grafo_lista(int n)
{
    if (n < 0) return NULL;
    GrafoLista *g = malloc(sizeof *g);
    if (!g) return NULL;
    g->n = n;
    g->adj = calloc((size_t)(n ? n : 1), sizeof *g->adj);
    if (!g->adj) { free(g); return NULL; }
    return g;
}

int inserir_aresta_lista(GrafoLista *g, int origem, int destino)
{
    if (!g || origem < 0 || destino < 0 || origem >= g->n ||
        destino >= g->n || origem == destino) return -1;
    for (No *a = g->adj[origem]; a; a = a->prox)
        if (a->destino == destino) return -1;
    No *a = malloc(sizeof *a), *b = malloc(sizeof *b);
    if (!a || !b) { free(a); free(b); return -1; }
    *a = (No){destino, g->adj[origem]};
    *b = (No){origem, g->adj[destino]};
    g->adj[origem] = a;
    g->adj[destino] = b;
    return 0;
}

void liberar_grafo_lista(GrafoLista *g)
{
    if (!g) return;
    for (int v = 0; v < g->n; ++v) {
        No *a = g->adj[v];
        while (a) { No *proximo = a->prox; free(a); a = proximo; }
    }
    free(g->adj);
    free(g);
}

/* ordem == NULL usa a ordem natural 0,1,...,n-1. */
static int *colorir(GrafoLista *g, const int ordem[], int *num_cores)
{
    size_t n = (size_t)(g->n ? g->n : 1);
    int *cores = malloc(n * sizeof *cores);
    int *proibida = malloc(n * sizeof *proibida);
    if (!cores || !proibida) { free(cores); free(proibida); return NULL; }
    for (int v = 0; v < g->n; ++v) cores[v] = proibida[v] = -1;
    for (int i = 0; i < g->n; ++i) {
        int v = ordem ? ordem[i] : i;
        /* A marca i evita limpar o array a cada vertice. */
        for (No *a = g->adj[v]; a; a = a->prox)
            if (cores[a->destino] >= 0) proibida[cores[a->destino]] = i;
        int cor = 0;
        while (cor < g->n && proibida[cor] == i) ++cor;
        cores[v] = cor;
        if (cor + 1 > *num_cores) *num_cores = cor + 1;
    }
    free(proibida);
    return cores;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores)
{
    if (!num_cores) return NULL;
    *num_cores = 0;
    if (!g || g->n < 0) return NULL;
    return colorir(g, NULL, num_cores);
}

typedef struct { int vertice, grau; } VerticeGrau;

static int comparar_graus(const void *a, const void *b)
{
    const VerticeGrau *x = a, *y = b;
    if (x->grau != y->grau) return x->grau > y->grau ? -1 : 1;
    /* Desempate pelo menor indice para obter resultado reprodutivel. */
    return (x->vertice > y->vertice) - (x->vertice < y->vertice);
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores)
{
    if (!num_cores) return NULL;
    *num_cores = 0;
    if (!g || g->n < 0) return NULL;
    size_t n = (size_t)(g->n ? g->n : 1);
    VerticeGrau *vertices = calloc(n, sizeof *vertices);
    int *ordem = malloc(n * sizeof *ordem);
    if (!vertices || !ordem) { free(vertices); free(ordem); return NULL; }
    for (int v = 0; v < g->n; ++v) {
        vertices[v].vertice = v;
        for (No *a = g->adj[v]; a; a = a->prox) ++vertices[v].grau;
    }
    qsort(vertices, (size_t)g->n, sizeof *vertices, comparar_graus);
    for (int i = 0; i < g->n; ++i) ordem[i] = vertices[i].vertice;
    int *cores = colorir(g, ordem, num_cores);
    free(vertices);
    free(ordem);
    return cores;
}

int eh_bipartido(GrafoLista *g)
{
    if (!g || g->n < 0) return -1;
    size_t n = (size_t)(g->n ? g->n : 1);
    int *cores = malloc(n * sizeof *cores);
    int *fila = malloc(n * sizeof *fila);
    if (!cores || !fila) { free(cores); free(fila); return -1; }
    for (int v = 0; v < g->n; ++v) cores[v] = -1;
    int resultado = 1;
    for (int raiz = 0; raiz < g->n && resultado; ++raiz) {
        if (cores[raiz] != -1) continue;
        int inicio = 0, fim = 0;
        cores[raiz] = 0;
        fila[fim++] = raiz;
        while (inicio < fim && resultado) {
            int u = fila[inicio++];
            for (No *a = g->adj[u]; a; a = a->prox) {
                int v = a->destino;
                if (cores[v] == -1) {
                    cores[v] = 1 - cores[u];
                    fila[fim++] = v;
                } else if (cores[v] == cores[u]) {
                    resultado = 0;
                    break;
                }
            }
        }
    }
    free(cores);
    free(fila);
    return resultado;
}

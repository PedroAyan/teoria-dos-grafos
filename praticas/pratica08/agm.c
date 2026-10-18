#include "agm.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

DSU *criar_dsu(int n)
{
    if (n <= 0) return NULL;
    DSU *d = malloc(sizeof *d);
    if (!d) return NULL;
    d->pai = malloc((size_t)n * sizeof *d->pai);
    d->rank = calloc((size_t)n, sizeof *d->rank);
    if (!d->pai || !d->rank) { liberar_dsu(d); return NULL; }
    d->n = n;
    for (int i = 0; i < n; ++i) d->pai[i] = i;
    return d;
}

void liberar_dsu(DSU *d)
{
    if (!d) return;
    free(d->pai);
    free(d->rank);
    free(d);
}

int find(DSU *d, int x)
{
    if (!d || x < 0 || x >= d->n) return -1;
    if (d->pai[x] != x) d->pai[x] = find(d, d->pai[x]);
    return d->pai[x];
}

int unir(DSU *d, int x, int y)
{
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);
    if (raiz_x < 0 || raiz_y < 0) return -1;
    if (raiz_x == raiz_y) return 0;
    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else {
        d->pai[raiz_y] = raiz_x;
        if (d->rank[raiz_x] == d->rank[raiz_y]) ++d->rank[raiz_x];
    }
    return 1;
}

static int entrada_valida(int n, const Aresta *arestas, int m,
                          Aresta *arvore, int *quantidade, long long *peso)
{
    if (n <= 0 || m < 0 || (m > 0 && !arestas) ||
        !arvore || !quantidade || !peso) return 0;
    for (int i = 0; i < m; ++i)
        if (arestas[i].u < 0 || arestas[i].u >= n ||
            arestas[i].v < 0 || arestas[i].v >= n) return 0;
    return 1;
}

/* Merge sort garante O(E log E), inclusive no pior caso.
 * Empates mantem a ordem da entrada. Nao se subtraem pesos para comparar. */
static void ordenar(Aresta *a, Aresta *aux, int inicio, int fim)
{
    if (fim - inicio < 2) return;
    int meio = inicio + (fim - inicio) / 2;
    ordenar(a, aux, inicio, meio);
    ordenar(a, aux, meio, fim);
    int i = inicio, j = meio, k = inicio;
    while (i < meio && j < fim)
        aux[k++] = a[i].peso <= a[j].peso ? a[i++] : a[j++];
    while (i < meio) aux[k++] = a[i++];
    while (j < fim) aux[k++] = a[j++];
    for (k = inicio; k < fim; ++k) a[k] = aux[k];
}

int kruskal(int n, const Aresta *arestas, int m, Aresta *arvore,
            int *quantidade, long long *peso_total)
{
    if (!entrada_valida(n, arestas, m, arvore, quantidade, peso_total)) return -1;
    *quantidade = 0;
    *peso_total = 0;
    DSU *d = criar_dsu(n);
    size_t capacidade = m > 0 ? (size_t)m : 1;
    Aresta *copia = malloc(capacidade * sizeof *copia);
    Aresta *aux = malloc(capacidade * sizeof *aux);
    if (!d || !copia || !aux) {
        liberar_dsu(d); free(copia); free(aux);
        return -1;
    }
    if (m > 0) memcpy(copia, arestas, (size_t)m * sizeof *copia);
    ordenar(copia, aux, 0, m);
    for (int i = 0; i < m && *quantidade < n - 1; ++i) {
        /* So aceita arestas entre componentes diferentes: nao cria ciclo. */
        if (unir(d, copia[i].u, copia[i].v) == 1) {
            arvore[(*quantidade)++] = copia[i];
            *peso_total += copia[i].peso;
        }
    }
    liberar_dsu(d);
    free(copia);
    free(aux);
    return *quantidade == n - 1;
}

typedef struct NoAdj {
    int destino, peso;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    int vertice;
    long long chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int *posicao;
    int tamanho;
} MinHeap;

static void trocar(MinHeap *h, int i, int j)
{
    NoHeap temp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = temp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

/* posicao encontra o vertice em O(1); subir custa O(log V). */
static void diminuir_chave(MinHeap *h, int vertice, long long chave)
{
    int i = h->posicao[vertice];
    h->dados[i].chave = chave;
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].chave <= h->dados[i].chave) break;
        trocar(h, pai, i);
        i = pai;
    }
}

static NoHeap extrair_minimo(MinHeap *h)
{
    NoHeap minimo = h->dados[0];
    h->posicao[minimo.vertice] = -1;
    --h->tamanho;
    if (h->tamanho == 0) return minimo;
    h->dados[0] = h->dados[h->tamanho];
    h->posicao[h->dados[0].vertice] = 0;
    int i = 0;
    while (i < h->tamanho / 2) {
        int filho = 2 * i + 1;
        if (filho + 1 < h->tamanho &&
            h->dados[filho + 1].chave < h->dados[filho].chave) ++filho;
        if (h->dados[i].chave <= h->dados[filho].chave) break;
        trocar(h, i, filho);
        i = filho;
    }
    return minimo;
}

int prim(int n, const Aresta *arestas, int m, int origem, Aresta *arvore,
         int *quantidade, long long *peso_total)
{
    if (!entrada_valida(n, arestas, m, arvore, quantidade, peso_total) ||
        origem < 0 || origem >= n) return -1;
    *quantidade = 0;
    *peso_total = 0;
    NoAdj **adj = calloc((size_t)n, sizeof *adj);
    size_t capacidade = m > 0 ? (size_t)m * 2 : 1;
    NoAdj *nos = malloc(capacidade * sizeof *nos);
    int *pred = malloc((size_t)n * sizeof *pred);
    MinHeap h = {malloc((size_t)n * sizeof *h.dados),
                 malloc((size_t)n * sizeof *h.posicao), n};
    int resultado = -1;
    if (!adj || !nos || !pred || !h.dados || !h.posicao) goto fim;

    /* Lista de adjacencia em O(V+E); cada aresta tem dois sentidos. */
    for (int i = 0; i < m; ++i) {
        int u = arestas[i].u, v = arestas[i].v;
        size_t j = (size_t)i * 2;
        nos[j] = (NoAdj){v, arestas[i].peso, adj[u]};
        adj[u] = &nos[j];
        nos[j + 1] = (NoAdj){u, arestas[i].peso, adj[v]};
        adj[v] = &nos[j + 1];
    }
    /* Todas as chaves iguais a infinito ja formam um heap valido. */
    for (int v = 0; v < n; ++v) {
        pred[v] = -1;
        h.dados[v] = (NoHeap){v, LLONG_MAX};
        h.posicao[v] = v;
    }
    diminuir_chave(&h, origem, 0);
    while (h.tamanho > 0) {
        NoHeap minimo = extrair_minimo(&h);
        int u = minimo.vertice;
        if (minimo.chave == LLONG_MAX) break; /* Componente esgotada. */
        if (pred[u] != -1) {
            arvore[(*quantidade)++] = (Aresta){pred[u], u, (int)minimo.chave};
            *peso_total += minimo.chave;
        }
        for (NoAdj *a = adj[u]; a; a = a->prox) {
            int pos = h.posicao[a->destino];
            if (pos >= 0 && a->peso < h.dados[pos].chave) {
                pred[a->destino] = u;
                diminuir_chave(&h, a->destino, a->peso);
            }
        }
    }
    resultado = *quantidade == n - 1;
fim:
    free(adj);
    free(nos);
    free(pred);
    free(h.dados);
    free(h.posicao);
    return resultado;
}

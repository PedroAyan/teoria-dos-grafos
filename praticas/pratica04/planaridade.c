#include "planaridade.h"
#include <stddef.h>

int eh_planar_euler(GrafoLista *g)
{
    if (!g || g->n < 0) return -1;
    if (g->n < 3) return 1;
    size_t m = 0;
    for (int u = 0; u < g->n; ++u)
        for (No *a = g->adj[u]; a; a = a->prox)
            if (u < a->destino) ++m;
    return m <= 3 * (size_t)g->n - 6;
}

typedef struct {
    int n, adj[10][10], grau[10];
    int ramos[6], quantidade_ramos, bipartido;
    int origem[10], destino[10], arestas;
    unsigned mascara_ramos;
} Busca;

static int ligar_arestas(Busca *b, int indice, unsigned usados);

/* Enumera caminhos simples. Vertices internos nao podem ser ramos nem
 * pertencer a outro caminho: as arestas do modelo viram caminhos
 * internamente disjuntos, exatamente a definicao de subdivisao. */
static int caminho(Busca *b, int indice, int u, unsigned usados)
{
    int destino = b->destino[indice];
    if (b->adj[u][destino] && ligar_arestas(b, indice + 1, usados)) return 1;
    for (int v = 0; v < b->n; ++v) {
        unsigned bit = 1u << v;
        if (b->adj[u][v] && !(bit & (b->mascara_ramos | usados)))
            if (caminho(b, indice, v, usados | bit)) return 1;
    }
    return 0;
}

static int ligar_arestas(Busca *b, int indice, unsigned usados)
{
    if (indice == b->arestas) return 1;
    /* Cada par sem aresta direta precisa de pelo menos um interno livre. */
    int faltam = 0, livres = 0;
    for (int i = indice; i < b->arestas; ++i)
        if (!b->adj[b->origem[i]][b->destino[i]]) ++faltam;
    for (int v = 0; v < b->n; ++v)
        if (!((b->mascara_ramos | usados) & (1u << v))) ++livres;
    if (faltam > livres) return 0;
    return caminho(b, indice, b->origem[indice], usados);
}

static int testar_modelo(Busca *b, unsigned lado)
{
    b->arestas = 0;
    for (int i = 0; i < b->quantidade_ramos; ++i)
        for (int j = i + 1; j < b->quantidade_ramos; ++j) {
            if (b->bipartido && ((lado >> i) & 1u) == ((lado >> j) & 1u))
                continue;
            b->origem[b->arestas] = b->ramos[i];
            b->destino[b->arestas++] = b->ramos[j];
        }
    return ligar_arestas(b, 0, 0);
}

static int escolher_ramos(Busca *b, int inicio, int quantidade)
{
    if (quantidade == b->quantidade_ramos) {
        if (!b->bipartido) return testar_modelo(b, 0);
        /* Fixar o primeiro ramo no lado A evita repetir A/B e B/A. */
        for (int i = 1; i < 6; ++i)
            for (int j = i + 1; j < 6; ++j)
                if (testar_modelo(b, 1u | (1u << i) | (1u << j))) return 1;
        return 0;
    }
    int restantes = b->quantidade_ramos - quantidade;
    for (int v = inicio; v <= b->n - restantes; ++v) {
        if (b->grau[v] < (b->bipartido ? 3 : 4)) continue;
        b->ramos[quantidade] = v;
        b->mascara_ramos |= 1u << v;
        if (escolher_ramos(b, v + 1, quantidade + 1)) return 1;
        b->mascara_ramos &= ~(1u << v);
    }
    return 0;
}

int contem_subdivisao_kuratowski(GrafoLista *g)
{
    if (!g || g->n < 0 || g->n > 10) return -1;
    Busca b = {0};
    b.n = g->n;
    for (int u = 0; u < g->n; ++u)
        for (No *a = g->adj[u]; a; a = a->prox) {
            b.adj[u][a->destino] = 1;
            ++b.grau[u];
        }
    b.quantidade_ramos = 5;
    if (escolher_ramos(&b, 0, 0)) return 1;
    b.quantidade_ramos = 6;
    b.bipartido = 1;
    return escolher_ramos(&b, 0, 0);
}

int eh_planar(GrafoLista *g)
{
    int euler = eh_planar_euler(g);
    if (euler <= 0) return euler;
    if (g->n > 10) return 2;
    return !contem_subdivisao_kuratowski(g);
}

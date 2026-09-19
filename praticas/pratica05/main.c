#include "coloracao.h"
#include <stdio.h>
#include <stdlib.h>

static int coloracao_valida(const GrafoLista *g, const int cores[], int total)
{
    if (!cores || total < 1 || total > g->n) return 0;
    int *usadas = calloc((size_t)total, sizeof *usadas);
    if (!usadas) return 0;
    int correto = 1;
    for (int v = 0; v < g->n; ++v) {
        if (cores[v] < 0 || cores[v] >= total) { correto = 0; break; }
        usadas[cores[v]] = 1;
        for (No *a = g->adj[v]; a; a = a->prox)
            if (cores[v] == cores[a->destino]) correto = 0;
    }
    for (int cor = 0; cor < total; ++cor)
        if (!usadas[cor]) correto = 0;
    free(usadas);
    return correto;
}

static int demonstrar(const char *titulo, int n, const int arestas[][2], int m,
                      int gulosa_esperada, int welsh_esperada, int bipartido_esperado)
{
    GrafoLista *g = criar_grafo_lista(n);
    if (!g) return 0;
    for (int i = 0; i < m; ++i) {
        if (inserir_aresta_lista(g, arestas[i][0], arestas[i][1]) != 0) {
            liberar_grafo_lista(g);
            return 0;
        }
    }
    int total_gulosa, total_welsh;
    int *gulosa = coloracao_gulosa(g, &total_gulosa);
    int *welsh = coloracao_welsh_powell(g, &total_welsh);
    int bipartido = eh_bipartido(g);
    if (!gulosa || !welsh || bipartido < 0) {
        fprintf(stderr, "Erro ao processar o grafo.\n");
        free(gulosa);
        free(welsh);
        liberar_grafo_lista(g);
        return 0;
    }
    printf("\n%s\nArestas:", titulo);
    for (int i = 0; i < m; ++i) printf(" %d-%d", arestas[i][0], arestas[i][1]);
    printf("\nVertice | Gulosa | Welsh-Powell\n");
    for (int v = 0; v < n; ++v) printf("%7d | %6d | %12d\n", v, gulosa[v], welsh[v]);
    printf("Cores da gulosa: %d\nCores de Welsh-Powell: %d\nBipartido (BFS): %s\n",
           total_gulosa, total_welsh, bipartido ? "sim" : "nao");
    int correto = coloracao_valida(g, gulosa, total_gulosa) &&
                  coloracao_valida(g, welsh, total_welsh) &&
                  total_gulosa == gulosa_esperada && total_welsh == welsh_esperada &&
                  bipartido == bipartido_esperado;
    printf("Teste: %s\n", correto ? "OK" : "FALHOU");
    free(gulosa);
    free(welsh);
    liberar_grafo_lista(g);
    return correto;
}

int main(void)
{
    /* Apenas dois casos, conforme solicitado. A ordem natural pode gastar
     * tres cores neste caminho, embora duas sejam suficientes. */
    const int sem_ciclo[][2] = {{0,2}, {2,3}, {3,1}};
    int primeiro = demonstrar("Grafo sem ciclo: caminho 0-2-3-1", 4,
                              sem_ciclo, 3, 3, 2, 1);

    /* Triangulo 1-2-3-1 com folha 4 e vertice 0 isolado: a BFS precisa
     * examinar tambem a componente que nao contem o vertice zero. */
    const int com_ciclo[][2] = {{1,2}, {2,3}, {3,1}, {3,4}};
    int segundo = demonstrar("Grafo com ciclo: triangulo, folha e vertice isolado", 5,
                             com_ciclo, 4, 3, 3, 0);
    return primeiro && segundo ? EXIT_SUCCESS : EXIT_FAILURE;
}

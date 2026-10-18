#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

static int demonstrar(const char *titulo, int n, const int arestas[][3], int m,
                      const int esperado[], const int pred_esperado[])
{
    GrafoListaPonderado *g = criar_grafo_ponderado(n);
    int *dist = malloc((size_t)n * sizeof *dist);
    int *pred = malloc((size_t)n * sizeof *pred);
    int correto = 0;
    if (!g || !dist || !pred) goto fim;
    for (int i = 0; i < m; ++i)
        if (inserir_aresta_ponderada(g, arestas[i][0], arestas[i][1],
                                    arestas[i][2]) != 0) goto fim;
    if (dijkstra(g, 0, dist, pred) != 0) goto fim;

    printf("\n%s\nOrigem: 0\nArestas (origem, destino, peso):\n", titulo);
    for (int i = 0; i < m; ++i)
        printf("(%d, %d, %d)\n", arestas[i][0], arestas[i][1], arestas[i][2]);
    correto = 1;
    for (int v = 0; v < n; ++v) {
        printf("Vertice %d | distancia: ", v);
        if (dist[v] == INT_MAX) printf("INF");
        else printf("%d", dist[v]);
        printf(" | predecessor: %d | caminho: ", pred[v]);
        reconstruir_caminho(0, v, pred);
        if (dist[v] != esperado[v] || pred[v] != pred_esperado[v]) correto = 0;
    }
    printf("Teste: %s\n", correto ? "OK" : "FALHOU");
fim:
    if (!correto) fprintf(stderr, "Falha: %s\n", titulo);
    free(dist);
    free(pred);
    liberar_grafo_ponderado(g);
    return correto;
}

int main(void)
{
    /* Somente dois grafos, conforme solicitado. */
    const int sem_ciclo[][3] = {{0,1,4}, {1,2,2}, {1,3,1}, {3,4,3}};
    const int dist_sem[] = {0,4,6,5,8};
    const int pred_sem[] = {-1,0,1,1,3};
    int primeiro = demonstrar("Grafo sem ciclo", 5, sem_ciclo, 4,
                              dist_sem, pred_sem);

    /* O caminho 0-2-1 custa 3 e substitui a distancia inicial 10.
     * A aresta 3-4 tem peso zero; o vertice 5 esta isolado. */
    const int com_ciclo[][3] = {
        {0,1,10}, {0,2,1}, {2,1,2}, {1,3,1}, {2,3,8}, {3,4,0}
    };
    const int dist_com[] = {0,3,1,4,4,INT_MAX};
    const int pred_com[] = {-1,2,0,1,3,-1};
    int segundo = demonstrar("Grafo com ciclo", 6, com_ciclo, 6,
                             dist_com, pred_com);
    return primeiro && segundo ? EXIT_SUCCESS : EXIT_FAILURE;
}

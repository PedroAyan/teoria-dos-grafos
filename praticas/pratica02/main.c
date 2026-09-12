#include "busca_profundidade.h"
#include <stdio.h>
#include <stdlib.h>

static int adicionar_arestas(GrafoLista *g, const int arestas[][2], size_t quantidade)
{
    for (size_t i = 0; i < quantidade; ++i) {
        if (!inserir_aresta_lista(g, arestas[i][0], arestas[i][1])) {
            return 0;
        }
    }

    return 1;
}

static int executar_teste(const char *titulo, GrafoLista *g, int ciclo_esperado,
                          int bipartido_esperado)
{
    int n = g->n;
    int *dist = malloc((size_t)(n > 0 ? n : 1) * sizeof *dist);
    int *pred = malloc((size_t)(n > 0 ? n : 1) * sizeof *pred);
    int *visitado = calloc((size_t)(n > 0 ? n : 1), sizeof *visitado);
    int *entrada = calloc((size_t)(n > 0 ? n : 1), sizeof *entrada);
    int *saida = calloc((size_t)(n > 0 ? n : 1), sizeof *saida);
    if (dist == NULL || pred == NULL || visitado == NULL || entrada == NULL
        || saida == NULL) {
        free(dist);
        free(pred);
        free(visitado);
        free(entrada);
        free(saida);
        return 0;
    }

    printf("\n=== %s ===\n", titulo);

    int sucesso = bfs(g, 0, dist, pred);
    if (sucesso) {
        printf("BFS a partir de 0 (vertice: distancia, predecessor):\n");
        for (int v = 0; v < n; ++v) {
            printf("  %d: %d, %d\n", v, dist[v], pred[v]);
        }
    }

    int tempo = 0;
    dfs_recursiva(g, 0, visitado, &tempo, entrada, saida);
    printf("DFS (vertice: entrada/saida):\n");
    for (int v = 0; v < n; ++v) {
        printf("  %d: %d/%d\n", v, entrada[v], saida[v]);
    }

    int componentes = contar_componentes(g);
    int ciclo = tem_ciclo(g);
    int bipartido = eh_bipartido(g);
    printf("Componentes conexos: %d\n", componentes);
    printf("Tem ciclo: %s\n", ciclo == 1 ? "sim" : "nao");
    printf("Eh bipartido: %s\n", bipartido == 1 ? "sim" : "nao");

    sucesso = sucesso && componentes == 1 && ciclo == ciclo_esperado
              && bipartido == bipartido_esperado;

    free(dist);
    free(pred);
    free(visitado);
    free(entrada);
    free(saida);
    return sucesso;
}

int main(void)
{
    /* Teste essencial 1: grafo conexo sem ciclo. */
    GrafoLista *sem_ciclo = criar_grafo_lista(5);
    const int arestas_sem_ciclo[][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 4}};

    /* Teste essencial 2: grafo conexo com um ciclo. */
    GrafoLista *com_ciclo = criar_grafo_lista(5);
    const int arestas_com_ciclo[][2] = {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}};

    if (sem_ciclo == NULL || com_ciclo == NULL
        || !adicionar_arestas(sem_ciclo, arestas_sem_ciclo,
                              sizeof arestas_sem_ciclo / sizeof arestas_sem_ciclo[0])
        || !adicionar_arestas(com_ciclo, arestas_com_ciclo,
                              sizeof arestas_com_ciclo / sizeof arestas_com_ciclo[0])) {
        liberar_grafo_lista(sem_ciclo);
        liberar_grafo_lista(com_ciclo);
        fprintf(stderr, "Erro ao preparar os grafos de teste.\n");
        return EXIT_FAILURE;
    }

    int sucesso_sem_ciclo = executar_teste("Grafo sem ciclo", sem_ciclo, 0, 1);
    int sucesso_com_ciclo = executar_teste("Grafo com ciclo", com_ciclo, 1, 0);

    liberar_grafo_lista(sem_ciclo);
    liberar_grafo_lista(com_ciclo);

    return sucesso_sem_ciclo && sucesso_com_ciclo ? EXIT_SUCCESS : EXIT_FAILURE;
}

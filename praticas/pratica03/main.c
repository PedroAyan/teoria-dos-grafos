#include "dag.h"
#include <stdio.h>
#include <stdlib.h>

static int demonstrar(const char *titulo, GrafoLista *g)
{
    printf("\n%s\n", titulo);
    int resultado = eh_dag(g);
    if (resultado < 0) {
        fprintf(stderr, "Erro ao verificar o grafo.\n");
        return 0;
    }
    printf("Eh DAG? %s\n", resultado ? "Sim" : "Nao");
    if (!resultado) {
        int tamanho_kahn, tamanho_dfs;
        int *kahn = ordenacao_topologica_kahn(g, &tamanho_kahn);
        int *dfs = ordenacao_topologica_dfs(g, &tamanho_dfs);
        int correto = !kahn && !dfs && tamanho_kahn == 0 && tamanho_dfs == 0;
        free(kahn);
        free(dfs);
        puts("Ciclo detectado: ordenacao topologica impossivel.");
        return correto;
    }
    const char *nomes[] = {"Kahn", "DFS"};
    int *(*algoritmos[])(GrafoLista *, int *) = {
        ordenacao_topologica_kahn, ordenacao_topologica_dfs
    };
    for (int i = 0; i < 2; ++i) {
        int tamanho;
        int *ordem = algoritmos[i](g, &tamanho);
        if (!ordem) {
            fprintf(stderr, "Erro na ordenacao por %s.\n", nomes[i]);
            return 0;
        }
        printf("%s:", nomes[i]);
        for (int j = 0; j < tamanho; ++j) printf(" %d", ordem[j]);
        putchar('\n');
        free(ordem);
    }
    return 1;
}

int main(void)
{
    GrafoLista *g = criar_grafo_lista(6);
    if (!g) return EXIT_FAILURE;
    const int arestas[][2] = {{5, 2}, {5, 0}, {4, 0}, {4, 1}, {2, 3}, {3, 1}};
    for (size_t i = 0; i < sizeof arestas / sizeof arestas[0]; ++i) {
        if (!inserir_aresta_lista(g, arestas[i][0], arestas[i][1])) {
            liberar_grafo_lista(g);
            return EXIT_FAILURE;
        }
    }
    int sucesso = demonstrar("Grafo dirigido aciclico", g);
    /* Forma o ciclo 1 -> 5 -> 2 -> 3 -> 1. */
    if (!inserir_aresta_lista(g, 1, 5)) sucesso = 0;
    else if (!demonstrar("Mesmo grafo com a aresta 1 -> 5", g)) sucesso = 0;
    liberar_grafo_lista(g);
    return sucesso ? EXIT_SUCCESS : EXIT_FAILURE;
}
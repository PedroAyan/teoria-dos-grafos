#include "bellman_ford.h"
#include <stdio.h>
#include <stdlib.h>

/* Confere se a cadeia reconstruida forma um ciclo de peso negativo
 * usando arestas que realmente existem no grafo deste teste. */
static int validar_ciclo(const GrafoListaPonderado *g, const int *pred)
{
    int inicio = g->ultimo_relaxado;
    for (int i = 0; i < g->n; ++i) {
        if (inicio < 0 || inicio >= g->n) return 0;
        inicio = pred[inicio];
    }
    if (inicio < 0 || inicio >= g->n) return 0;
    int atual = inicio;
    long long soma = 0;
    int quantidade = 0;
    do {
        if (atual < 0 || atual >= g->n || ++quantidade > g->n) return 0;
        int anterior = pred[atual];
        if (anterior < 0 || anterior >= g->n) return 0;
        ArestaPonderada *a = g->adj[anterior];
        while (a && a->destino != atual) a = a->prox;
        if (!a) return 0;
        soma += a->peso;
        atual = anterior;
    } while (atual != inicio);
    printf("Peso do ciclo: %lld\n", soma);
    return soma == -2;
}

static int demonstrar(const char *titulo, int n, const int arestas[][3], int m,
                      int retorno_esperado, const int *dist_esperada,
                      const int *pred_esperado)
{
    GrafoListaPonderado *g = criar_grafo_ponderado(n);
    int *dist = malloc((size_t)n * sizeof *dist);
    int *pred = malloc((size_t)n * sizeof *pred);
    int correto = 0;
    if (!g || !dist || !pred) goto fim;
    for (int i = 0; i < m; ++i)
        if (inserir_aresta_ponderada(g, arestas[i][0], arestas[i][1],
                                    arestas[i][2]) != 0) goto fim;

    printf("\n%s\nOrigem: 0\nArestas (origem, destino, peso):\n", titulo);
    for (int i = 0; i < m; ++i)
        printf("(%d, %d, %d)\n", arestas[i][0], arestas[i][1], arestas[i][2]);
    int resultado = bellman_ford(g, 0, dist, pred);
    printf("Retorno de Bellman-Ford: %d\n", resultado);
    correto = resultado == retorno_esperado;
    if (resultado == 1) {
        for (int v = 0; v < n; ++v) {
            printf("Vertice %d | distancia: ", v);
            if (dist[v] == INT_MAX) printf("INF");
            else printf("%d", dist[v]);
            printf(" | predecessor: %d\n", pred[v]);
            if (!dist_esperada || !pred_esperado ||
                dist[v] != dist_esperada[v] || pred[v] != pred_esperado[v])
                correto = 0;
        }
        if (g->ultimo_relaxado != -1) correto = 0;
    } else if (resultado == 0) {
        int valido = validar_ciclo(g, pred);
        if (!valido || !reconstruir_ciclo_negativo(n, g->ultimo_relaxado, pred))
            correto = 0;
    } else {
        correto = 0;
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
    /* Somente um grafo sem ciclo e outro com ciclo, conforme solicitado. */
    const int sem_ciclo[][3] = {
        {0,1,4}, {0,2,5}, {1,2,-2}, {2,3,3}, {1,3,6}
    };
    const int dist_esperada[] = {0,4,2,5,INT_MAX};
    const int pred_esperado[] = {-1,0,1,2,-1};
    int primeiro = demonstrar("Grafo sem ciclo (vertice 4 isolado)", 5,
                              sem_ciclo, 5, 1, dist_esperada, pred_esperado);

    /* Ciclo 1 -> 2 -> 1 tem peso -3 + 1 = -2.
     * A testemunha pode ser o vertice 4, depois do ciclo. */
    const int com_ciclo[][3] = {
        {0,1,1}, {1,2,-3}, {2,1,1}, {2,3,2}, {3,4,1}
    };
    int segundo = demonstrar("Grafo com ciclo negativo alcancavel", 5,
                             com_ciclo, 5, 0, NULL, NULL);
    return primeiro && segundo ? EXIT_SUCCESS : EXIT_FAILURE;
}

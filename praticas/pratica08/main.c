#include "agm.h"
#include <stdio.h>
#include <stdlib.h>

static int validar_arvore(int n, const Aresta *entrada, int m,
                          const Aresta *arvore, int quantidade,
                          long long peso, long long esperado)
{
    if (quantidade != n - 1 || peso != esperado) return 0;
    DSU *d = criar_dsu(n);
    if (!d) return 0;
    long long soma = 0;
    int valido = 1;
    for (int i = 0; i < quantidade; ++i) {
        Aresta a = arvore[i];
        int existe = 0;
        for (int j = 0; j < m; ++j)
            if (a.peso == entrada[j].peso &&
                ((a.u == entrada[j].u && a.v == entrada[j].v) ||
                 (a.u == entrada[j].v && a.v == entrada[j].u))) existe = 1;
        if (!existe || unir(d, a.u, a.v) != 1) valido = 0;
        soma += a.peso;
    }
    for (int v = 1; v < n; ++v)
        if (find(d, v) != find(d, 0)) valido = 0;
    liberar_dsu(d);
    return valido && soma == peso;
}

static void imprimir(const char *nome, const Aresta *arvore, int quantidade,
                      long long peso)
{
    printf("%s:\n", nome);
    for (int i = 0; i < quantidade; ++i)
        printf("  %d -- %d | peso: %d\n", arvore[i].u, arvore[i].v, arvore[i].peso);
    printf("Arestas selecionadas: %d\nCusto total: %lld\n", quantidade, peso);
}

static int demonstrar(const char *titulo, int n, const Aresta *arestas, int m,
                      long long esperado)
{
    Aresta *arvore_k = malloc((size_t)n * sizeof *arvore_k);
    Aresta *arvore_p = malloc((size_t)n * sizeof *arvore_p);
    int correto = 0;
    if (!arvore_k || !arvore_p) goto fim;
    printf("\n%s\nArestas do grafo:\n", titulo);
    for (int i = 0; i < m; ++i)
        printf("  %d -- %d | peso: %d\n", arestas[i].u, arestas[i].v, arestas[i].peso);

    int qtd_k, qtd_p;
    long long peso_k, peso_p;
    int retorno_k = kruskal(n, arestas, m, arvore_k, &qtd_k, &peso_k);
    int retorno_p = prim(n, arestas, m, 0, arvore_p, &qtd_p, &peso_p);
    if (retorno_k != 1 || retorno_p != 1) goto fim;
    imprimir("Kruskal", arvore_k, qtd_k, peso_k);
    imprimir("Prim (origem 0)", arvore_p, qtd_p, peso_p);
    correto = validar_arvore(n, arestas, m, arvore_k, qtd_k, peso_k, esperado) &&
              validar_arvore(n, arestas, m, arvore_p, qtd_p, peso_p, esperado);
    printf("Custo minimo esperado: %lld\nTeste: %s\n", esperado,
           correto ? "OK" : "FALHOU");
fim:
    if (!correto) fprintf(stderr, "Falha: %s\n", titulo);
    free(arvore_k);
    free(arvore_p);
    return correto;
}

int main(void)
{
    /* Apenas dois grafos. No primeiro, o proprio grafo ja e a unica AGM. */
    const Aresta sem_ciclo[] = {{0,1,4}, {1,2,-2}, {1,3,0}, {3,4,3}};
    int primeiro = demonstrar("Grafo sem ciclo", 5, sem_ciclo, 4, 5);

    /* Kruskal descarta arestas que fecham ciclos. Prim deve atualizar
     * a chave de 1 de 10 para 2 e a de 3 de 8 para 3.
     * AGM: 0-2 (1), 2-1 (2), 1-3 (3), 3-4 (4); total 10. */
    const Aresta com_ciclo[] = {
        {0,1,10}, {0,2,1}, {2,1,2}, {1,3,3}, {2,3,8}, {3,4,4}, {2,4,9}
    };
    int segundo = demonstrar("Grafo com ciclo", 5, com_ciclo, 7, 10);
    return primeiro && segundo ? EXIT_SUCCESS : EXIT_FAILURE;
}

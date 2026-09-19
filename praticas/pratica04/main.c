#include "conectividade.h"
#include "planaridade.h"
#include <stdio.h>
#include <stdlib.h>

/* Exatamente dois casos: uma arvore e uma subdivisao de K3,3 com ciclo.
 * Esta ultima satisfaz Euler, mas nao e planar: exige Kuratowski. */
static int testar(const char *nome, int n, const int arestas[][2], int m,
                  const int esperadas[], const Ponte pontes_esperadas[],
                  int numero_pontes, int planar)
{
    GrafoLista *g = criar_grafo_lista(n);
    if (!g) return 0;
    for (int i = 0; i < m; ++i) {
        if (inserir_aresta_lista(g, arestas[i][0], arestas[i][1]) != 0) {
            liberar_grafo_lista(g);
            return 0;
        }
    }
    int articulacao[10];
    Ponte pontes[10];
    int quantidade = detectar_articulacoes(g, articulacao);
    int total = detectar_pontes(g, pontes);
    int euler = eh_planar_euler(g);
    int kuratowski = contem_subdivisao_kuratowski(g);
    int resultado = eh_planar(g);
    int correto = quantidade >= 0 && total == numero_pontes &&
                  euler == 1 && kuratowski == !planar && resultado == planar;
    int total_esperado = 0;
    printf("\n%s\nArticulacoes:", nome);
    if (quantidade >= 0) {
        for (int u = 0; u < n; ++u) {
            total_esperado += esperadas[u];
            if (articulacao[u]) printf(" %d", u);
            if (articulacao[u] != esperadas[u]) correto = 0;
        }
        if (quantidade != total_esperado) correto = 0;
    }
    printf("\nPontes:");
    for (int i = 0; i < total; ++i) {
        printf(" %d-%d", pontes[i].origem, pontes[i].destino);
    }
    for (int i = 0; i < numero_pontes; ++i) {
        int ocorrencias = 0;
        for (int j = 0; j < total; ++j) {
            int u = pontes[j].origem, v = pontes[j].destino;
            int a = pontes_esperadas[i].origem, b = pontes_esperadas[i].destino;
            if ((u == a && v == b) || (u == b && v == a)) ++ocorrencias;
        }
        if (ocorrencias != 1) correto = 0;
    }
    printf("\nLimite de Euler: %s\nSubdivisao de Kuratowski: %s\n"
           "Planar: %s\nTeste: %s\n", euler == 1 ? "satisfeito" : "falhou",
           kuratowski == 1 ? "sim" : "nao", resultado == 1 ? "sim" : "nao",
           correto ? "OK" : "FALHOU");
    liberar_grafo_lista(g);
    return correto;
}

int main(void)
{
    const int arvore[][2] = {{0,1}, {1,2}, {1,3}, {3,4}};
    const int articulacoes_arvore[] = {0,1,0,1,0};
    const Ponte pontes_arvore[] = {{0,1}, {1,2}, {1,3}, {3,4}};
    int sem_ciclo = testar("Grafo sem ciclo (arvore)", 5, arvore, 4,
                           articulacoes_arvore, pontes_arvore, 4, 1);

    /* Particoes {0,1,2}/{3,4,5}; subdivide 0-3 em 0-6-3.
     * Acrescenta a folha 7, para conferir uma ponte junto dos ciclos. */
    const int ciclico[][2] = {
        {0,6}, {6,3}, {0,4}, {0,5}, {1,3}, {1,4},
        {1,5}, {2,3}, {2,4}, {2,5}, {0,7}
    };
    const int articulacoes_ciclico[] = {1,0,0,0,0,0,0,0};
    const Ponte pontes_ciclico[] = {{0,7}};
    int com_ciclo = testar("Grafo com ciclo (subdivisao de K3,3 e folha)",
                           8, ciclico, 11, articulacoes_ciclico,
                           pontes_ciclico, 1, 0);
    return sem_ciclo && com_ciclo ? EXIT_SUCCESS : EXIT_FAILURE;
}

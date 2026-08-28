#include <stdio.h>

#include "grafo_lista.h"
#include "grafo_matriz.h"

int main(void)
{
    const int n = 5;

    GrafoMatriz *grafo_matriz = criar_grafo_matriz(n);
    GrafoLista *grafo_lista = criar_grafo_lista(n);

    if (grafo_matriz == NULL || grafo_lista == NULL) {
        printf("Erro ao criar os grafos.\n");

        liberar_grafo_matriz(grafo_matriz);
        liberar_grafo_lista(grafo_lista);

        return 1;
    }

    printf("=== Inserindo arestas ===\n");

    inserir_aresta_matriz(grafo_matriz, 0, 1);
    inserir_aresta_matriz(grafo_matriz, 0, 2);
    inserir_aresta_matriz(grafo_matriz, 1, 2);
    inserir_aresta_matriz(grafo_matriz, 1, 3);
    inserir_aresta_matriz(grafo_matriz, 3, 4);

    inserir_aresta_lista(grafo_lista, 0, 1);
    inserir_aresta_lista(grafo_lista, 0, 2);
    inserir_aresta_lista(grafo_lista, 1, 2);
    inserir_aresta_lista(grafo_lista, 1, 3);
    inserir_aresta_lista(grafo_lista, 3, 4);

    printf("Arestas inseridas com sucesso.\n");

    printf("\n=== Verificando adjacencia ===\n");

    printf(
        "Matriz: vertices 0 e 1 sao adjacentes? %d\n",
        sao_adjacentes_matriz(grafo_matriz, 0, 1)
    );

    printf(
        "Lista: vertices 0 e 1 sao adjacentes? %d\n",
        sao_adjacentes_lista(grafo_lista, 0, 1)
    );

    printf(
        "Matriz: vertices 0 e 4 sao adjacentes? %d\n",
        sao_adjacentes_matriz(grafo_matriz, 0, 4)
    );

    printf(
        "Lista: vertices 0 e 4 sao adjacentes? %d\n",
        sao_adjacentes_lista(grafo_lista, 0, 4)
    );

    printf("\n=== Grau dos vertices ===\n");

    for (int i = 0; i < n; i++) {
        printf(
            "Vertice %d -> matriz: %d | lista: %d\n",
            i,
            grau_matriz(grafo_matriz, i),
            grau_lista(grafo_lista, i)
        );
    }

    printf("\n=== Removendo aresta (1, 2) ===\n");

    remover_aresta_matriz(grafo_matriz, 1, 2);
    remover_aresta_lista(grafo_lista, 1, 2);

    printf(
        "Matriz: vertices 1 e 2 sao adjacentes? %d\n",
        sao_adjacentes_matriz(grafo_matriz, 1, 2)
    );

    printf(
        "Lista: vertices 1 e 2 sao adjacentes? %d\n",
        sao_adjacentes_lista(grafo_lista, 1, 2)
    );

    printf("\nGrau do vertice 1 apos remocao:\n");

    printf(
        "Matriz: %d\n",
        grau_matriz(grafo_matriz, 1)
    );

    printf(
        "Lista: %d\n",
        grau_lista(grafo_lista, 1)
    );

    liberar_grafo_matriz(grafo_matriz);
    liberar_grafo_lista(grafo_lista);

    printf("\nMemoria liberada com sucesso.\n");

    return 0;
}
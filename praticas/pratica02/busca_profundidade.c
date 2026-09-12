#include "busca_profundidade.h"

#include <stdlib.h>

static int vertice_valido(const GrafoLista *g, int vertice)
{
    return g != NULL && vertice >= 0 && vertice < g->n;
}

static int fila_inicializar(Fila *fila, int capacidade)
{
    if (fila == NULL || capacidade <= 0) {
        return 0;
    }

    fila->dados = malloc((size_t)capacidade * sizeof *fila->dados);
    if (fila->dados == NULL) {
        return 0;
    }

    fila->capacidade = capacidade;
    fila->inicio = 0;
    fila->fim = 0;
    fila->tamanho = 0;
    return 1;
}

static void fila_liberar(Fila *fila)
{
    if (fila != NULL) {
        free(fila->dados);
        fila->dados = NULL;
    }
}

static int fila_inserir(Fila *fila, int valor)
{
    if (fila == NULL || fila->tamanho == fila->capacidade) {
        return 0;
    }

    fila->dados[fila->fim] = valor;
    fila->fim = (fila->fim + 1) % fila->capacidade;
    ++fila->tamanho;
    return 1;
}

static int fila_remover(Fila *fila, int *valor)
{
    if (fila == NULL || valor == NULL || fila->tamanho == 0) {
        return 0;
    }

    *valor = fila->dados[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    --fila->tamanho;
    return 1;
}

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo,
                   int *entrada, int *saida)
{
    if (!vertice_valido(g, u) || visitado == NULL || visitado[u]) {
        return;
    }

    visitado[u] = 1;
    if (tempo != NULL && entrada != NULL) {
        entrada[u] = ++(*tempo);
    }

    for (No *vizinho = g->adj[u]; vizinho != NULL; vizinho = vizinho->prox) {
        if (!visitado[vizinho->destino]) {
            dfs_recursiva(g, vizinho->destino, visitado, tempo, entrada, saida);
        }
    }

    if (tempo != NULL && saida != NULL) {
        saida[u] = ++(*tempo);
    }
}

int contar_componentes(GrafoLista *g)
{
    if (g == NULL || g->n < 0) {
        return -1;
    }

    int *visitado = calloc((size_t)(g->n > 0 ? g->n : 1), sizeof *visitado);
    if (visitado == NULL) {
        return -1;
    }

    int componentes = 0;
    for (int v = 0; v < g->n; ++v) {
        if (visitado[v]) {
            continue;
        }

        ++componentes;
        dfs_recursiva(g, v, visitado, NULL, NULL, NULL);
    }

    free(visitado);
    return componentes;
}

static int dfs_ciclo(GrafoLista *g, int u, int pai, int *visitado)
{
    visitado[u] = 1;

    for (No *vizinho = g->adj[u]; vizinho != NULL; vizinho = vizinho->prox) {
        int v = vizinho->destino;
        if (!visitado[v]) {
            if (dfs_ciclo(g, v, u, visitado)) {
                return 1;
            }
        } else if (v != pai) {
            return 1;
        }
    }

    return 0;
}

int tem_ciclo(GrafoLista *g)
{
    if (g == NULL || g->n < 0) {
        return -1;
    }

    int *visitado = calloc((size_t)(g->n > 0 ? g->n : 1), sizeof *visitado);
    if (visitado == NULL) {
        return -1;
    }

    int resultado = 0;
    for (int v = 0; v < g->n && !resultado; ++v) {
        if (!visitado[v]) {
            resultado = dfs_ciclo(g, v, -1, visitado);
        }
    }

    free(visitado);
    return resultado;
}

int eh_bipartido(GrafoLista *g)
{
    if (g == NULL || g->n < 0) {
        return -1;
    }

    int *cor = malloc((size_t)(g->n > 0 ? g->n : 1) * sizeof *cor);
    if (cor == NULL) {
        return -1;
    }

    for (int v = 0; v < g->n; ++v) {
        cor[v] = -1;
    }

    Fila fila;
    if (!fila_inicializar(&fila, g->n > 0 ? g->n : 1)) {
        free(cor);
        return -1;
    }

    int resultado = 1;
    for (int origem = 0; origem < g->n && resultado; ++origem) {
        if (cor[origem] != -1) {
            continue;
        }

        cor[origem] = 0;
        if (!fila_inserir(&fila, origem)) {
            resultado = -1;
            break;
        }

        int atual;
        while (resultado == 1 && fila_remover(&fila, &atual)) {
            for (No *vizinho = g->adj[atual]; vizinho != NULL;
                 vizinho = vizinho->prox) {
                int destino = vizinho->destino;
                if (cor[destino] == -1) {
                    cor[destino] = 1 - cor[atual];
                    if (!fila_inserir(&fila, destino)) {
                        resultado = -1;
                        break;
                    }
                } else if (cor[destino] == cor[atual]) {
                    resultado = 0;
                    break;
                }
            }
        }
    }

    fila_liberar(&fila);
    free(cor);
    return resultado;
}

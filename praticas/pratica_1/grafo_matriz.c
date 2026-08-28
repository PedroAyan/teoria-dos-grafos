#include <stdio.h>
#include <stdlib.h>

#include "grafo_matriz.h"

static int indice_valido(const RedeMatricial *r, int vertice)
{
    return vertice >= 0 && vertice < r->total_vertices;
}

RedeMatricial *nova_rede_matricial(int total_vertices)
{
    if (total_vertices < 0) {
        return NULL;
    }

    RedeMatricial *r = malloc(sizeof(RedeMatricial));
    if (r == NULL) {
        return NULL;
    }

    r->total_vertices = total_vertices;
    r->matriz = NULL;

    if (total_vertices == 0) {
        return r;
    }

    r->matriz = malloc((size_t)total_vertices * sizeof(int *));
    if (r->matriz == NULL) {
        free(r);
        return NULL;
    }

    for (int i = 0; i < total_vertices; i++) {
        r->matriz[i] = calloc((size_t)total_vertices, sizeof(int));
        if (r->matriz[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(r->matriz[j]);
            }
            free(r->matriz);
            free(r);
            return NULL;
        }
    }

    return r;
}

int adicionar_ligacao_matricial(RedeMatricial *r, int a, int b)
{
    if (r == NULL || !indice_valido(r, a) || !indice_valido(r, b)) {
        return -1;
    }

    if (r->matriz[a][b] == 1) {
        return -1;
    }

    r->matriz[a][b] = 1;
    r->matriz[b][a] = 1;

    return 0;
}

int excluir_ligacao_matricial(RedeMatricial *r, int a, int b)
{
    if (r == NULL || !indice_valido(r, a) || !indice_valido(r, b)) {
        return -1;
    }

    if (r->matriz[a][b] == 0) {
        return -1;
    }

    r->matriz[a][b] = 0;
    r->matriz[b][a] = 0;

    return 0;
}

int quantidade_conexoes_matricial(const RedeMatricial *r, int vertice)
{
    if (r == NULL || !indice_valido(r, vertice)) {
        return -1;
    }

    int total = 0;
    for (int i = 0; i < r->total_vertices; i++) {
        total += r->matriz[vertice][i];
    }

    return total;
}

int existe_ligacao_matricial(const RedeMatricial *r, int a, int b)
{
    if (r == NULL || !indice_valido(r, a) || !indice_valido(r, b)) {
        return -1;
    }

    return r->matriz[a][b];
}

void destruir_rede_matricial(RedeMatricial *r)
{
    if (r == NULL) {
        return;
    }

    if (r->matriz != NULL) {
        for (int i = 0; i < r->total_vertices; i++) {
            free(r->matriz[i]);
        }
        free(r->matriz);
    }

    free(r);
}
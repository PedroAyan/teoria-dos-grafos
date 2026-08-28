#include <stdio.h>
#include <stdlib.h>

#include "rede_encadeada.h"

static int indice_valido(const RedeEncadeada r, int vertice)
{
    return vertice >= 0 && vertice < r->total_vertices;
}

static Elocriar_elo(int alvo)
{
    Elo elo = malloc(sizeof(Elo));
    if (elo == NULL) {
        return NULL;
    }

    elo->alvo = alvo;
    elo->seguinte = NULL;

    return elo;
}

static int contem_alvo(const Elolista, int alvo)
{
    for (const Elo atual = lista; atual != NULL; atual = atual->seguinte) {
        if (atual->alvo == alvo) {
            return 1;
        }
    }

    return 0;
}

/ Remove o primeiro elo com alvo == destino da lista de r->cabecas[origem].
 
Retorna 0 se removeu, -1 se nao encontrou o elo. */
static int remover_elo(RedeEncadeada r, int origem, int destino)
{
    Eloatual = r->cabecas[origem];
    Elo anterior = NULL;

    while (atual != NULL) {
        if (atual->alvo == destino) {
            if (anterior == NULL) {
                r->cabecas[origem] = atual->seguinte;
            } else {
                anterior->seguinte = atual->seguinte;
            }
            free(atual);
            return 0;
        }
        anterior = atual;
        atual = atual->seguinte;
    }

    return -1;
}

RedeEncadeadanova_rede_encadeada(int total_vertices)
{
    if (total_vertices < 0) {
        return NULL;
    }

    RedeEncadeada r = malloc(sizeof(RedeEncadeada));
    if (r == NULL) {
        return NULL;
    }

    r->total_vertices = total_vertices;
    r->cabecas = NULL;

    if (total_vertices == 0) {
        return r;
    }

    r->cabecas = malloc((size_t)total_vertices sizeof(Elo *));
    if (r->cabecas == NULL) {
        free(r);
        return NULL;
    }

    for (int i = 0; i < total_vertices; i++) {
        r->cabecas[i] = NULL;
    }

    return r;
}
int adicionar_ligacao_encadeada(RedeEncadeada r, int a, int b)
{
    if (r == NULL  !indice_valido(r, a) 
 !indice_valido(r, b)) {
        return -1;
    }

    if (contem_alvo(r->cabecas[a], b)) {
        return -1;
    }

    Eloelo_a = criar_elo(b);
    if (elo_a == NULL) {
        return -1;
    }
    elo_a->seguinte = r->cabecas[a];
    r->cabecas[a] = elo_a;

    if (a != b) {
        Elo elo_b = criar_elo(a);
        if (elo_b == NULL) {
            remover_elo(r, a, b);
            return -1;
        }
        elo_b->seguinte = r->cabecas[b];
        r->cabecas[b] = elo_b;
    }

    return 0;
}

int excluir_ligacao_encadeada(RedeEncadeadar, int a, int b)
{
    if (r == NULL  !indice_valido(r, a) 
 !indice_valido(r, b)) {
        return -1;
    }

    if (remover_elo(r, a, b) != 0) {
        return -1;
    }

    if (a != b) {
        remover_elo(r, b, a);
    }

    return 0;
}

int quantidade_conexoes_encadeada(const RedeEncadeada r, int vertice)
{
    if (r == NULL || !indice_valido(r, vertice)) {
        return -1;
    }

    int total = 0;
    for (const Eloatual = r->cabecas[vertice]; atual != NULL; atual = atual->seguinte) {
        total++;
    }

    return total;
}

int existe_ligacao_encadeada(const RedeEncadeada r, int a, int b)
{
    if (r == NULL  !indice_valido(r, a) 
 !indice_valido(r, b)) {
        return -1;
    }

    return contem_alvo(r->cabecas[a], b);
}

void destruir_rede_encadeada(RedeEncadeadar)
{
    if (r == NULL) {
        return;
    }

    if (r->cabecas != NULL) {
        for (int i = 0; i < r->total_vertices; i++) {
            Elo atual = r->cabecas[i];
            while (atual != NULL) {
                Eloproximo = atual->seguinte;
                free(atual);
                atual = proximo;
            }
        }
        free(r->cabecas);
    }

    free(r);
}
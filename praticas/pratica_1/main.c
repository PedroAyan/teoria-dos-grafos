#include <stdio.h>

#include "grafo_matriz.h"
#include "grafo_lista.h"

static void secao(const char *titulo)
{
    printf("\n=== %s ===\n", titulo);
}

int main(void)
{
    const int VERTICES = 5;

    secao("Criacao das redes");
    RedeMatricial *rm = nova_rede_matricial(VERTICES);
    RedeEncadeada *re = nova_rede_encadeada(VERTICES);

    if (rm == NULL || re == NULL) {
        printf("Falha ao criar as redes.\n");
        destruir_rede_matricial(rm);
        destruir_rede_encadeada(re);
        return 1;
    }
    printf("Rede matricial criada com %d vertices.\n", rm->total_vertices);
    printf("Rede encadeada criada com %d vertices.\n", re->total_vertices);

    secao("Adicao de ligacoes");
    int ligacoes[][2] = { {0, 1}, {0, 2}, {1, 2}, {1, 3}, {3, 4} };
    int total_ligacoes = (int)(sizeof(ligacoes) / sizeof(ligacoes[0]));

    for (int i = 0; i < total_ligacoes; i++) {
        int a = ligacoes[i][0];
        int b = ligacoes[i][1];
        int ok_m = adicionar_ligacao_matricial(rm, a, b);
        int ok_e = adicionar_ligacao_encadeada(re, a, b);
        printf("Adicionar (%d, %d) -> matricial: %s | encadeada: %s\n",
               a, b, ok_m == 0 ? "OK" : "FALHOU", ok_e == 0 ? "OK" : "FALHOU");
    }

    secao("Consulta de ligacao");
    int consultas[][2] = { {0, 1}, {0, 3}, {3, 4} };
    for (int i = 0; i < 3; i++) {
        int a = consultas[i][0];
        int b = consultas[i][1];
        printf("Ligados (%d, %d) -> matricial: %d | encadeada: %d\n",
               a, b, existe_ligacao_matricial(rm, a, b), existe_ligacao_encadeada(re, a, b));
    }

    secao("Consulta de quantidade de conexoes");
    for (int v = 0; v < VERTICES; v++) {
        printf("Conexoes do vertice %d -> matricial: %d | encadeada: %d\n",
               v, quantidade_conexoes_matricial(rm, v), quantidade_conexoes_encadeada(re, v));
    }

    secao("Adicao de ligacao duplicada");
    printf("Adicionar (0, 1) novamente -> matricial: %s | encadeada: %s\n",
           adicionar_ligacao_matricial(rm, 0, 1) == 0 ? "OK" : "FALHOU (duplicada)",
           adicionar_ligacao_encadeada(re, 0, 1) == 0 ? "OK" : "FALHOU (duplicada)");

    secao("Exclusao de ligacao");
    printf("Excluir (1, 2) -> matricial: %s | encadeada: %s\n",
           excluir_ligacao_matricial(rm, 1, 2) == 0 ? "OK" : "FALHOU",
           excluir_ligacao_encadeada(re, 1, 2) == 0 ? "OK" : "FALHOU");

    secao("Consulta apos exclusao");
    printf("Ligados (1, 2) -> matricial: %d | encadeada: %d\n",
           existe_ligacao_matricial(rm, 1, 2), existe_ligacao_encadeada(re, 1, 2));
    printf("Conexoes do vertice 1 -> matricial: %d | encadeada: %d\n",
           quantidade_conexoes_matricial(rm, 1), quantidade_conexoes_encadeada(re, 1));

    secao("Exclusao de ligacao inexistente");
    printf("Excluir (1, 2) novamente -> matricial: %s | encadeada: %s\n",
           excluir_ligacao_matricial(rm, 1, 2) == 0 ? "OK" : "FALHOU (inexistente)",
           excluir_ligacao_encadeada(re, 1, 2) == 0 ? "OK" : "FALHOU (inexistente)");
    printf("Excluir (0, 4) (nunca existiu) -> matricial: %s | encadeada: %s\n",
           excluir_ligacao_matricial(rm, 0, 4) == 0 ? "OK" : "FALHOU (inexistente)",
           excluir_ligacao_encadeada(re, 0, 4) == 0 ? "OK" : "FALHOU (inexistente)");

    secao("Casos relevantes da rede encadeada");
    printf("Conexoes do vertice 1 antes das exclusoes -> encadeada: %d\n",
           quantidade_conexoes_encadeada(re, 1));
    printf("Excluir o elo inserido por ultimo -> na verdade o primeiro da lista (1, 3) -> encadeada: %s\n",
           excluir_ligacao_encadeada(re, 1, 3) == 0 ? "OK" : "FALHOU");
    printf("Excluir elo intermediario (0, 1) -> encadeada: %s\n",
           excluir_ligacao_encadeada(re, 0, 1) == 0 ? "OK" : "FALHOU");
    printf("Conexoes do vertice 1 apos exclusoes -> encadeada: %d\n",
           quantidade_conexoes_encadeada(re, 1));

    secao("Vertice invalido e ponteiro nulo");
    printf("Conexoes do vertice 10 (invalido) -> matricial: %d | encadeada: %d\n",
           quantidade_conexoes_matricial(rm, 10), quantidade_conexoes_encadeada(re, 10));
    printf("Ligados (-1, 0) (invalido) -> matricial: %d | encadeada: %d\n",
           existe_ligacao_matricial(rm, -1, 0), existe_ligacao_encadeada(re, -1, 0));
    printf("Conexoes em rede nula -> matricial: %d | encadeada: %d\n",
           quantidade_conexoes_matricial(NULL, 0), quantidade_conexoes_encadeada(NULL, 0));

    secao("Rede vazia (total_vertices = 0)");
    RedeMatricial *rm_vazia = nova_rede_matricial(0);
    RedeEncadeada *re_vazia = nova_rede_encadeada(0);
    printf("Rede vazia criada -> matricial: %s | encadeada: %s\n",
           rm_vazia != NULL ? "OK" : "FALHOU", re_vazia != NULL ? "OK" : "FALHOU");
    printf("Adicao em rede vazia -> matricial: %s | encadeada: %s\n",
           adicionar_ligacao_matricial(rm_vazia, 0, 0) == 0 ? "OK" : "FALHOU (esperado)",
           adicionar_ligacao_encadeada(re_vazia, 0, 0) == 0 ? "OK" : "FALHOU (esperado)");

    secao("Criacao invalida (total_vertices < 0)");
    RedeMatricial *rm_invalida = nova_rede_matricial(-1);
    RedeEncadeada *re_invalida = nova_rede_encadeada(-1);
    printf("Criar com total_vertices = -1 -> matricial: %s | encadeada: %s\n",
           rm_invalida == NULL ? "NULL (esperado)" : "ERRO",
           re_invalida == NULL ? "NULL (esperado)" : "ERRO");

    secao("Liberacao da memoria");
    destruir_rede_matricial(rm);
    destruir_rede_encadeada(re);
    destruir_rede_matricial(rm_vazia);
    destruir_rede_encadeada(re_vazia);
    destruir_rede_matricial(rm_invalida);
    destruir_rede_encadeada(re_invalida);
    destruir_rede_matricial(NULL);
    destruir_rede_encadeada(NULL);
    printf("Toda a memoria alocada foi liberada.\n");

    return 0;
}
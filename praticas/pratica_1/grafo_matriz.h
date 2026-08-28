#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

/* Rede nao direcionada representada por matriz de adjacencia.
 * Vertices numerados de 0 a total_vertices-1. */
typedef struct {
    int total_vertices;
    int **matriz;
} RedeMatricial;

/* Cria uma rede com total_vertices vertices (0 gera uma rede vazia valida).
 * Retorna NULL se total_vertices < 0 ou se a alocacao falhar. */
RedeMatricial *nova_rede_matricial(int total_vertices);

/* Adiciona a ligacao entre a e b. Retorna 0 em sucesso, -1 se a rede for
 * nula, algum vertice for invalido ou a ligacao ja existir. */
int adicionar_ligacao_matricial(RedeMatricial *r, int a, int b);

/* Exclui a ligacao entre a e b. Retorna 0 em sucesso, -1 se a rede for
 * nula, algum vertice for invalido ou a ligacao nao existir. */
int excluir_ligacao_matricial(RedeMatricial *r, int a, int b);

/* Retorna a quantidade de conexoes do vertice, ou -1 se a rede for nula
 * ou o vertice for invalido. */
int quantidade_conexoes_matricial(const RedeMatricial *r, int vertice);

/* Retorna 1 se a e b estao ligados, 0 se nao estao, ou -1 se a rede
 * for nula ou algum vertice for invalido. */
int existe_ligacao_matricial(const RedeMatricial *r, int a, int b);

/* Libera toda a memoria alocada para a rede. Aceita r == NULL sem efeito. */
void destruir_rede_matricial(RedeMatricial *r);

#endif
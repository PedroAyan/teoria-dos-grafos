#ifndef AGM_H
#define AGM_H

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);
/* Vertices validos: 0 a n-1. find retorna -1 para entrada invalida. */
int find(DSU *d, int x);
/* Retorna 1 se uniu, 0 se ja estavam juntos, -1 para entrada invalida. */
int unir(DSU *d, int x, int y);

/* Grafo nao direcionado: cada aresta aparece uma unica vez na entrada.
 * Pesos negativos, zero e positivos sao permitidos.
 * arvore deve ter espaco para pelo menos n-1 arestas (uma posicao se n=1).
 * Retorno: 1 = AGM encontrada, 0 = grafo desconexo, -1 = erro.
 * quantidade e peso_total descrevem as arestas selecionadas.
 * Com retorno 0, nao existe AGM de todo o grafo: Kruskal retorna uma
 * floresta minima e Prim retorna a arvore da componente da origem.
 * A entrada arestas nao e modificada. */
int kruskal(int n, const Aresta *arestas, int m, Aresta *arvore,
            int *quantidade, long long *peso_total);
int prim(int n, const Aresta *arestas, int m, int origem, Aresta *arvore,
         int *quantidade, long long *peso_total);

#endif

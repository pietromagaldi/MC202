#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grafo *p_grafo;

struct grafo {
    int **adj;
    int n_vertices;
    int *vertices; // tem valor 1 quando o vertice correspondente à posição está no grafo
};

typedef struct fila *p_fila;

struct fila{
    int *dados;
    int tamanho_limite, tamanho_atual;
    int ini, fim; // indices 
};

p_fila cria_fila(){
    p_fila fila;
    fila = malloc(sizeof(struct fila));
    fila->dados = malloc((22) * sizeof(int));
    fila->ini = 0;
    fila->fim = 0;
    fila->tamanho_atual = 0;
    fila->tamanho_limite = 22;
    return fila;
}

void enfileira(p_fila fila, int dado){
    fila->dados[fila->fim] = dado;
    (fila->fim)++;
    (fila->tamanho_atual)++;
}

int desenfileira(p_fila fila){
    int dado = fila->dados[fila->ini];
    for (int i=1; i < (fila->tamanho_atual) + 1 ; i++){
        fila->dados[i-1] = fila->dados[i];
    }
    (fila->fim)--;
    (fila->tamanho_atual)--;
    return dado;
}

void libera_fila(p_fila fila){
    free(fila->dados);
    free(fila);
}

p_grafo nasce_o_grafo(int n_max){
    int i, j;
    p_grafo grafo = malloc(sizeof(struct grafo));
    grafo->n_vertices = n_max;
    grafo->adj = malloc(n_max*sizeof(int *));
    grafo->vertices = malloc(n_max*sizeof(int));
    for (i=0; i < n_max; i++){
        grafo->adj[i] = malloc(n_max*sizeof(int));
        grafo->vertices[i] = 0;
        for (j = 0; j < n_max; j++){
            grafo->adj[i][j] = 0;
        }
    }
    return grafo;
}

void mata_grafo(p_grafo grafo){
    for (int i = 0; i < grafo->n_vertices; i++){
        free(grafo->adj[i]);
    }
    free(grafo->adj);
    free(grafo->vertices);
    free(grafo);
}

void modifica_aresta(p_grafo grafo, int v1, int v2, int marcador){
    if (marcador == 1){ // adiciona aresta
        grafo->adj[v1][v2] = grafo->adj[v2][v1] = 1;
    }
    else if (marcador == -1){ // remove aresta
        grafo->adj[v1][v2] = grafo->adj[v2][v1] = 0;
    }
}

int busca_rec(p_grafo g, int *visitado , int v, int t) {
    int w;
    if (v == t)
        return 1; /*sempre existe caminho de t para t*/
    visitado[v] = 1;
    for (w = 0; w < g->n_vertices; w++)
        if (g->adj[v][w] && !visitado[w])
            if (busca_rec(g, visitado , w, t))
                return 1;
    return 0;
}

int existe_caminho(p_grafo g, int s, int t) {
    int encontrou , i, *visitado = malloc(g->n_vertices * sizeof(int));
    for (i = 0; i < g->n_vertices; i++)
        visitado[i] = 0;
    encontrou = busca_rec(g, visitado , s, t);
    free(visitado);
    return encontrou;
}

void adiciona_vertice(p_grafo grafo, int vertice){
    grafo->vertices[vertice] = 1;
}

void encontra_conexão(p_grafo grafo){
    int destino;
    for (int v = 1; v < grafo->n_vertices; v++){
        if (grafo->vertices[v] == 1){
            printf("Ponto %d (Pontos diretamente ou indiretamente conectados):", v);
            for (destino = 0; destino < grafo->n_vertices; destino++){
                if (grafo->vertices[destino] == 1 && existe_caminho(grafo, v, destino)){
                    printf(" %d", destino);
                }
            }
            printf("\n");
        }   
    }
}

void imprime(int *pai, int v1, int v2){ // imprime o menor caminho entre v1 e v2
    int v_atual = v2;
    if (v_atual != pai[v_atual]){
        imprime(pai, v1, pai[v_atual]);
        printf(" -> %d", v_atual);
    }
    else{
        printf("%d", v_atual);
    }
}

void busca_em_largura(p_grafo g, int s, int v2) {
    int w, v;
    int *pai = malloc(g->n_vertices * sizeof(int));
    int *visitado = malloc(g->n_vertices * sizeof(int));
    p_fila f = cria_fila();
    for (v = 0; v < g->n_vertices; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }
    enfileira(f,s);
    pai[s] = s;
    visitado[s] = 1;
    while(f->tamanho_atual>0) {
        v = desenfileira(f);
        for (w = 0; w < g->n_vertices; w++)
        if (g->adj[v][w] && !visitado[w]) {
            visitado[w] = 1; /*evita repetição na fila*/
            pai[w] = v;
            enfileira(f, w);
        }
    }
    free(f->dados);
    free(f);
    free(visitado);
    imprime(pai, s, v2);
    free(pai);
}


int main(){
    int comando = 0;
    p_grafo grafo;
    grafo = nasce_o_grafo(22);
    int vertice1 = 0, vertice2 = 0;
    while (1){
        scanf("%d", &comando);
        if (comando == 1){
            scanf("%d", &vertice1);
            adiciona_vertice(grafo, vertice1);
        }
        else if (comando == 2){
            scanf("%d %d", &vertice1, &vertice2);
            modifica_aresta(grafo, vertice1, vertice2, 1);
        }
        else if (comando == 3){
            scanf("%d %d", &vertice1, &vertice2);
            modifica_aresta(grafo, vertice1, vertice2, -1);
        }
        else if (comando == 4){
            int encontrou = 0; 
            scanf("%d %d", &vertice1, &vertice2);
            encontrou = existe_caminho(grafo, vertice1, vertice2);
            if (encontrou == 0){
                printf("Não existe conexão entre os nós.\n");
            }
            else if (encontrou == 1){
                printf("Existe conexão entre os nós.\n");
            }
        }
        else if (comando == 5){
            scanf("%d %d", &vertice1, &vertice2);
            if (existe_caminho(grafo, vertice1, vertice2)){
                busca_em_largura(grafo, vertice1, vertice2);
                printf("\n");
            }
            else{
                printf("Não existe conexão entre os nós.\n");
            }
        }
        else if (comando == 6){
            encontra_conexão(grafo);
        }
        else {
            mata_grafo(grafo);
            break;
        }
    }
    return 0;
}
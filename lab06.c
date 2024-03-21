#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no *p_no;

struct no {
    char livro[56];
    int marcador; // serve de marcador para saber aonde preciso inserir um livro
    p_no prox;
};

p_no cria_lista(){
    return NULL;
}

void imprime_lista(p_no lista){
    p_no atual;
    for (atual = lista; atual != NULL; atual = atual->prox){
        if (atual->prox == NULL){
            printf("%s", atual->livro);
        }
        else{
            printf("%s, ", atual->livro);
        }
    }
    printf("\n");
}

p_no adicionar_elemento(p_no lista, char livro[56], int k){ // k=0 -> inicio, k=1 -> final da lista
    p_no novo;
    novo = lista;
    if (novo == NULL){
        novo = malloc(sizeof(struct no));
        strcpy(novo->livro, livro);
        novo->marcador = 0;
        novo->prox = lista;
        return novo;
    }
    else if (k == 0){
        if (lista->marcador == 1){
            novo = malloc(sizeof(struct no));
            strcpy(novo->livro, livro);
            novo->prox = lista;
            novo->marcador = 0;
            return novo;
        }
        else{
            for (; novo != NULL; novo = novo->prox){
                if (novo->prox->marcador == 1){
                    p_no aux, inserido;
                    inserido = malloc(sizeof(struct no));
                    aux = novo->prox;
                    strcpy(inserido->livro, livro);
                    inserido->prox = aux;
                    inserido->marcador = 0;
                    novo->prox = inserido;
                    break;
                }
            }
            return lista;
        }
    }
    else if (k == 1){
        for (; novo != NULL; novo = novo->prox){
            if (novo->prox == NULL){
                novo->prox = malloc(sizeof(struct no));
                strcpy(novo->prox->livro, livro);
                novo->prox->prox = NULL;
                novo->prox->marcador = 0;
                break;
            }
        }
        return lista;
    }
    return lista;
}

p_no remover_elemento(p_no lista, char livro[56]){
    p_no novo, aux;
    int cont=0; // em que posicao estamos na lista
    novo = lista;
    for (; novo != NULL; novo = novo->prox){
        if (cont == 0 && strcmp(novo->livro, livro) == 0){ // o livro a ser removido esta na primeira pos da lista
            aux = novo;
            lista = novo->prox;
            if (novo->marcador == 1){
                novo->prox->marcador = 1;
            }
            free(aux);
            return lista;
        }
        else if (strcmp(novo->prox->livro, livro) == 0){
            if (novo->prox->prox == NULL){ // o livro a ser removido esta na ultima pos da lista
                aux = novo->prox;
                novo->prox = NULL;
                free(aux);
                return lista;
                // adicionar prox no final
            }
            else{
                aux = novo->prox;
                if (novo->prox->marcador == 1){
                    novo->prox->prox->marcador = 1;
                }
                novo->prox = novo->prox->prox;
                free(aux);
                return lista;
                // adicionar prox atras do novo->prox->prox (que tá com o marcador 1)
            }
        }
        cont++;
    }
    return lista;
}

void da_free(p_no lista){
    if (lista != NULL) {
    da_free(lista->prox);
    free(lista);
    }
}

int main(){
    p_no lista_de_leitura;
    lista_de_leitura = cria_lista();
    char leitura[56], comando[10];
    int k=1; // por definicao, os livros sao adicionados no final da lista
        while (scanf("%s ", comando) != EOF){
            if (comando[1] == 'm'){ // comando=imprimir
                imprime_lista(lista_de_leitura);
            }
            else if (comando[1] == 'n'){ // comando=inicio
                k = 0;
                lista_de_leitura->marcador = 1;
            }
            else if (comando[1] == 'i'){ // comando=final
                k = 1;
            }
            else if (comando[1] == 'e'){ // comando=remover
                fgets(leitura, sizeof(leitura), stdin);
                strtok(leitura, "\n");
                lista_de_leitura = remover_elemento(lista_de_leitura, leitura);
            }
            else{ // comando=adicionar
                fgets(leitura, sizeof(leitura), stdin);
                strtok(leitura, "\n");
                lista_de_leitura = adicionar_elemento(lista_de_leitura, leitura, k);
            }
    }
    da_free(lista_de_leitura);
    return 0;
}
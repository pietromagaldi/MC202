#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 120

typedef struct no *p_no_a;

struct no {
    char dado;
    p_no_a dir;
    p_no_a esq;
};

// def pilha

typedef struct pilha *p_pilha;

typedef struct no_p *p_no_p;

struct no_p{
    p_no_a dado;
    p_no_p prox;
};

struct pilha{
    p_no_p topo;
};

// funcoes pilha 

p_pilha cria_pilha(){
    p_pilha pilha;
    pilha = malloc(sizeof(struct pilha));
    pilha->topo = NULL;
    return pilha;
}

void empilha(p_pilha pilha, p_no_a no_atual){
    p_no_p novo = malloc(sizeof(struct no_p));
    novo->dado = no_atual;
    novo->prox = pilha->topo;
    pilha->topo = novo;
}

p_no_a desempilha(p_pilha pilha){
    p_no_p aux = pilha->topo;
    p_no_a no_atual = aux->dado;
    if (pilha->topo->prox == NULL){
        pilha->topo = NULL;
    }
    else{
        pilha->topo = pilha->topo->prox;
    }
    free(aux);
    return no_atual;
}

void libera_pilha(p_pilha pilha){
    free(pilha);
}

p_no_a cria_no(char dado){
    p_no_a r = malloc(sizeof(struct no));
    r->dado = dado;
    r->dir = NULL;
    r->esq = NULL;
    return r;
}

int eh_operador(char c){
    if (c == '{' || c == '}'|| c == '>'|| c == '<'
    || c == '&'|| c == '|' || c == '=' || c == '!'){
        return 1;
    }
    return 0;
}

int eh_numero(char c){
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4'
    || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'){
        return 1;
    }
    return 0;
}

p_no_a cria_arvore(char expressao[MAX], p_pilha pilha){
    char char_atual;
    p_no_a no_atual;
    for (int j = 0; j<strlen(expressao); j++){
        char_atual = expressao[j];
        no_atual = cria_no(char_atual);
        if (!eh_operador(char_atual)){
            empilha(pilha, no_atual); // empilha o cara se for numero ou letra
        }
        else{ // quando lemos um operador, desempilhamos dois nós, de modo que esses nós sejam os filhos desse operador
            p_no_a dir = desempilha(pilha);
            p_no_a esq = desempilha(pilha);
            no_atual->dir = dir;
            no_atual->esq = esq;
            empilha(pilha, no_atual); // empilha o nó com os filhos
        }
    }
    return pilha->topo->dado;
}

void inordem(p_no_a raiz) { // adicionar parenteses 
    if (raiz != NULL) {
        if (eh_operador(raiz->dado)){
            printf("(");
        }
        inordem(raiz->esq);
        if (eh_operador(raiz->dado)){
            printf(" %c ", raiz->dado);
        }
        else{
            printf("%c", raiz->dado);
        }    
        inordem(raiz->dir);
        if (eh_operador(raiz->dado)){
            printf(")");
        }

    }
}

void desmatamento(p_no_a raiz){ // mata a arvore
    if (raiz != NULL){
        desmatamento(raiz->esq);
        desmatamento(raiz->dir);
        free(raiz);
    }
}

void otimiza(p_no_a raiz){
    if (eh_operador(raiz->esq->dado)){
        otimiza(raiz->esq);
    }
    if (eh_operador(raiz->dir->dado)){
        otimiza(raiz->dir);
    }
    if (raiz->dado == '{'){ 
        if (raiz->esq->dado == raiz->dir->dado && !eh_operador(raiz->esq->dado)){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (raiz->dir->dado != '9' && !eh_numero(raiz->esq->dado)){
            // nao faz nada
        }
        else if (!eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else if (raiz->dir->dado == '9' || raiz->esq->dado <= raiz->dir->dado){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
    else if (raiz->dado == '}'){
        if (raiz->esq->dado == raiz->dir->dado && !eh_operador(raiz->esq->dado)){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (raiz->dir->dado != '0' && !eh_numero(raiz->esq->dado)){
            // nao faz nada
        }
        else if (raiz->esq->dado == '9'){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (!eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else if (raiz->dir->dado == '0' || raiz->esq->dado >= raiz->dir->dado){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
    else if (raiz->dado == '>'){
        if (raiz->esq->dado == raiz->dir->dado && !eh_operador(raiz->esq->dado)){
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (raiz->esq->dado == '0' || raiz->dir->dado == '9'){
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (!eh_numero(raiz->esq->dado) || !eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else if (raiz->esq->dado > raiz->dir->dado){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
    else if (raiz->dado == '<'){
        if (raiz->esq->dado == raiz->dir->dado && !eh_operador(raiz->esq->dado)){
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (raiz->esq->dado == '9' || raiz->dir->dado == '0'){
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (!eh_numero(raiz->esq->dado) || !eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else if (raiz->esq->dado < raiz->dir->dado){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
    else if (raiz->dado == '&'){
        if (raiz->esq->dado == 'T' && raiz->dir->dado == 'T'){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (raiz->esq->dado == 'F' || raiz->dir->dado == 'F'){
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (!eh_numero(raiz->esq->dado) || !eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
    else if (raiz->dado == '|'){
        if (raiz->esq->dado == 'T' || raiz->dir->dado == 'T'){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (!eh_numero(raiz->esq->dado) || !eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
    else if (raiz->dado == '='){
        if (raiz->esq->dado == raiz->dir->dado && !eh_operador(raiz->esq->dado)){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (!eh_numero(raiz->esq->dado) || !eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
    else if (raiz->dado == '!'){
        if (raiz->esq->dado == raiz->dir->dado && !eh_operador(raiz->esq->dado)){
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else if (!eh_numero(raiz->esq->dado) || !eh_numero(raiz->dir->dado)){
            // nao faz nada
        }
        else if (raiz->esq->dado != raiz->dir->dado){
            raiz->dado = 'T';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
        else{
            raiz->dado = 'F';
            desmatamento(raiz->dir);
            desmatamento(raiz->esq);
            raiz->esq = raiz->dir = NULL;
        }
    }
}

int main(){
    int n_expressoes;
    char expressao[MAX];
    p_pilha pilha;
    p_no_a arvore;
    pilha = cria_pilha();
    scanf("%d ", &n_expressoes);
    for (int i=0; i<n_expressoes; i++){
        scanf("%s ", expressao);
        arvore = cria_arvore(expressao, pilha);
        inordem(arvore);
        printf("\n");
        otimiza(arvore);
        inordem(arvore);
        printf("\n\n");
        desmatamento(arvore);
        free(pilha->topo);
    }
    libera_pilha(pilha);
    return 0;
}
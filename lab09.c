#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200
enum cor {VERMELHO , PRETO};

typedef struct no *p_no;

struct no{
    char *nome;
    char *tipo;
    enum cor cor;
    p_no esq, dir;
};

int eh_vermelho(p_no raiz){
    if (raiz == NULL){
        return 0;
    }
    return raiz->cor == VERMELHO;
}

int eh_preto(p_no raiz){
    if (raiz == NULL){
        return 1;
    }
    return raiz->cor == PRETO;
}

p_no rotaciona_p_esq(p_no raiz){
    p_no dir = raiz->dir;
    raiz->dir = dir->esq;
    dir->esq = raiz;
    dir->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return dir;
}

p_no rotaciona_p_dir(p_no raiz){
    p_no esq = raiz->esq;
    raiz->esq = esq->dir;
    esq->dir = raiz;
    esq->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return esq;
}

void sobe_vermelho(p_no raiz){
    raiz->cor = VERMELHO;
    raiz->dir->cor = raiz->esq->cor = PRETO;
}

p_no cria_no(char nome[MAX], char tipo[7]){
    p_no novo;
    novo = malloc(sizeof(struct no));
    novo->esq = novo->dir = NULL;
    novo->nome = malloc(MAX*sizeof(char));
    novo->tipo = malloc(7*sizeof(char));
    strcpy(novo->nome, nome);
    strcpy(novo->tipo, tipo);
    novo->cor = VERMELHO;
    return novo;
}

p_no inserir_rec(p_no raiz, char nome[MAX], char tipo[7]) {
    if (raiz == NULL) {
        raiz = cria_no(nome, tipo);
        return raiz;
    }
    if (strcmp(nome, raiz->nome) > 0){
        raiz->esq = inserir_rec(raiz->esq, nome, tipo);
    }
    else{
        raiz->dir = inserir_rec(raiz->dir, nome, tipo);
    }
    /* corrige a árvore */
    if (eh_vermelho(raiz->dir) && eh_preto(raiz->esq)){
        raiz = rotaciona_p_esq(raiz);
    }
    if (eh_vermelho(raiz->esq) && eh_vermelho(raiz->esq->esq)){
        raiz = rotaciona_p_dir(raiz);
    }
    if (eh_vermelho(raiz->esq) && eh_vermelho(raiz->dir)){
        sobe_vermelho(raiz);
    }
    return raiz;
    }

void desmatamento(p_no raiz){ // mata a arvore
    if (raiz != NULL){
        desmatamento(raiz->esq);
        desmatamento(raiz->dir);
        free(raiz->nome);
        free(raiz->tipo);
        free(raiz);
    }
}

p_no buscar(p_no raiz, char nome[MAX]) {
    while (raiz != NULL && strcmp(nome, raiz->nome)){
        if (strcmp(nome, raiz->nome) > 0){
            raiz = raiz->esq;
        }
        else{
            raiz = raiz->dir;
        }
    }
    return raiz;
}

void compara(p_no a, p_no b, p_no c, int n_linha_atual, int *contador, char op){
    if (strcmp(b->tipo, c->tipo)){ // se b e c tem tipos diferentes
        printf("Linha %d: tipos incompatíveis: %s (%s) %c %s (%s).\n", n_linha_atual, b->nome, b->tipo, op, c->nome, c->tipo);
        *contador = *contador + 1;
    }
    else if (strcmp(a->tipo, b->tipo)){ // se a e b tem tipos diferentes
        printf("Linha %d: tipos incompatíveis: o símbolo %s espera %s, obteve %s.\n", n_linha_atual, a->nome, a->tipo, b->tipo);
        *contador = *contador + 1;
    }
    else if (strcmp(a->tipo, c->tipo)){ // se a e c tem tipos diferentes
        printf("Linha %d: tipos incompatíveis: o símbolo %s espera %s, obteve %s.\n", n_linha_atual, a->nome, a->tipo, c->tipo);
        *contador = *contador + 1;
    }
}

int eh_declaracao(char linha[MAX]){
    char palavra[MAX];
    sscanf(linha, "%s ", palavra);
    if (!strcmp(palavra, "int") || !strcmp(palavra, "char") || !strcmp(palavra, "double") || !strcmp(palavra, "float")){
        // caso a linha comesse com um tipo, ela é uma declaração (já que não há parênteses, o que já foi verificado antes)
        return 1;
    }
    return 0;
}

int pulo_ou_nao(char linha[MAX]){
    char palavra[MAX];
    sscanf(linha, "%s ", palavra);
    if (!strcmp(palavra, "return")){
        return 1;
    }
    else if (linha[0] == '#' || linha[0] == '\n'){ // condições pra pular linha
        return 1;
    }
    for (int i=0; i < strlen(linha); i++){
        if (linha[i] == '(' || linha[i] == '}' || linha[i] == '{'){ // verificar se há parênteses na linha
            return 1;
        }
    }
    return 0;
}

void imprime(p_no raiz){
    if (raiz != NULL){
        imprime(raiz->dir);
        printf("%s %s\n", raiz->nome, raiz->tipo);
        imprime(raiz->esq);
    }
}

int main(){
    char linha_atual[MAX];
    int n_linha_atual = 0;
    int contador = 0;
    p_no arvore = NULL;
    while (1){
        n_linha_atual++;
        fgets(linha_atual, MAX, stdin);
        if (!pulo_ou_nao(linha_atual)){ // se não precisa pular a linha atual
            
            if (eh_declaracao(linha_atual)){ // se minha linha atual é declaração de uma variável
                char nome[MAX], tipo[7];
                sscanf(linha_atual, "%s %s", tipo, nome); // scaneia o tipo e o nome da variável
                strtok(nome, ";"); // tira o ";" do fim do nome da variável
                arvore = inserir_rec(arvore, nome, tipo); // insere a variável na árvore rubro-negra
            }

            else{ // é expressão
                char n_a[MAX], n_b[MAX], n_c[MAX]; // operação sempre é do tipo a = b + c (n_a, n_b, n_c são nomes das variáveis)
                char op1, op2; // operador 1, operador 2
                sscanf(linha_atual, "%s %c %s %c %s", n_a, &op1, n_b, &op2, n_c);
                strtok(n_a, ";");
                strtok(n_b, ";");
                strtok(n_c, ";");
                if (!buscar(arvore, n_a)){ // verifica se o nome n_a está na árvore
                    printf("Linha %d: o símbolo %s não foi definido.\n", n_linha_atual, n_a);
                }
                else if (!buscar(arvore, n_b)){ // mesma coisa para n_b
                    printf("Linha %d: o símbolo %s não foi definido.\n", n_linha_atual, n_b);
                }
                else if (!buscar(arvore, n_c)){ // mesma coisa para n_c
                    printf("Linha %d: o símbolo %s não foi definido.\n", n_linha_atual, n_c);
                }
                else{
                    p_no a, b, c;
                    a = buscar(arvore, n_a); // atribui os nós às variáveis
                    b = buscar(arvore, n_b);
                    c = buscar(arvore, n_c);
                    compara(a, b, c, n_linha_atual, &contador, op2); // compara os nós
                }
            }
        }
        if (feof(stdin)){
            if (contador == 0){
                printf("Não há erros de tipo.\n");
            }
            imprime(arvore);
            desmatamento(arvore);
            break;
        }
    }
    return 0;
}
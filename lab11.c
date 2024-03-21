#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM 2999

typedef struct familia *p_familia;

struct familia {
    char *nome;
    int doces;
    int removido;
};

typedef struct hash *p_hash;

struct hash {
    p_familia *vetor;
};

p_hash nasce_o_hash(){
    p_hash hash;
    hash = malloc(sizeof(struct hash));
    hash->vetor = malloc(TAM*sizeof(struct familia));
    for (int i=0; i<TAM; i++){
        hash->vetor[i] = NULL; // no início todos os campos são vazios (NULL)
    }
    return hash;
}

p_familia nasce_a_familia(char nome[20], int doces){
    p_familia fam;
    fam = malloc(sizeof(struct familia));
    fam->nome = malloc(20*sizeof(char));
    strcpy(fam->nome, nome);
    fam->doces = doces;
    fam->removido = 0;
    return fam;    
}

int hashing(char nome[20]){
    int i, n = 0;
    for (i = 0; i < strlen(nome); i++)
        n = (256 * n + nome[i]) % TAM; 
    return n;
}

void libera_hash(p_hash hash){
    for (int i=0; i<TAM; i++){
        if (hash->vetor[i] != NULL){
            free(hash->vetor[i]->nome);
            free(hash->vetor[i]);
        }
    }
    free(hash->vetor);
    free(hash);
}

int busca(p_hash hash, char nome[20]){
    int posicao = hashing(nome);
    int i = posicao;
    if (!strcmp(hash->vetor[posicao]->nome, nome)){ // se o nome for igual ao que eu to procurando
        return posicao;
    }
    else{
        while (1){
            if (!strcmp(hash->vetor[i]->nome, nome)){ // se o nome for igual ao que eu to procurando
                return i;
            }
            else if (i+1 == TAM){ // se ultrapassar do tamanho, volta a procurar no começo do hash
                i = 0;
            }
            i++;
        }
    }
    return 0;
}

void insere(p_hash hash, p_familia fam){
    int posicao = hashing(fam->nome);
    int i = posicao;
    if (hash->vetor[posicao] == NULL || hash->vetor[posicao]->removido == 1){ // dá pra colocar
        hash->vetor[posicao] = fam;
    }
    else{
        while (1){
            if (hash->vetor[i] == NULL || hash->vetor[i]->removido == 1){
                hash->vetor[i] = fam;
                break;
            }
            else if (i+1 == TAM){ // se passar do tamanho do hash, volta a tentar inserir no começo
                i = 0;
            }
            i++;
        }
    }
}

void remover(p_hash hash, char nome[20]){ 
    int posicao = busca(hash, nome);
    printf("A familia %s saiu com %d doce(s) sobrando.\n", hash->vetor[posicao]->nome, hash->vetor[posicao]->doces);
    hash->vetor[posicao]->removido = 1;
}

void comprar(p_hash hash, char nome[20], int quant){
    int posicao = busca(hash, nome);
    hash->vetor[posicao]->doces += quant;
    printf("A familia %s agora possui %d doces.\n", hash->vetor[posicao]->nome, hash->vetor[posicao]->doces);
}

int tira_doce(p_hash hash, char familia[20], char nota[10]){
    int posicao = busca(hash, familia);
    int tirei = 0; // quantos doces foram tirados da família
    if (hash->vetor[posicao]->doces != 0){
        if (!strcmp(nota, "Boa")){
            if (hash->vetor[posicao]->doces <= 3){
                tirei = hash->vetor[posicao]->doces; // tira o restante de doces da família
                hash->vetor[posicao]->doces = 0;
                printf("A familia %s ficou sem doces.\n", hash->vetor[posicao]->nome);
            }
            else{
                hash->vetor[posicao]->doces -= 3;
                tirei = 3;
            }
        }
        else if(!strcmp(nota, "Media")){
            if (hash->vetor[posicao]->doces <= 2){
                tirei = hash->vetor[posicao]->doces; 
                hash->vetor[posicao]->doces = 0;
                printf("A familia %s ficou sem doces.\n", hash->vetor[posicao]->nome);
            }
            else{
                hash->vetor[posicao]->doces -= 2;
                tirei = 2;
            }
        }
        else if(!strcmp(nota, "Ruim")){
            if (hash->vetor[posicao]->doces <= 1){
                tirei = 0;
                printf("A familia %s ficou sem doces.\n", hash->vetor[posicao]->nome);
            }
            else{
                hash->vetor[posicao]->doces -= 1;
                tirei = 1;
            }
        }
    }
    return tirei;
}

void trick_or_treat(p_hash hash, char nome[20], char sobrenome[20], int qtd_casas){
    char familia[20];
    char nota[10];
    int doces = 0; // soma dos doces recebidos pela criança
    for (int i=0; i<qtd_casas; i++){
        scanf("%s %s ", familia, nota);
        doces += tira_doce(hash, familia, nota);
    }
    if (doces == 0){
        printf("%s %s recebeu 10 doces da prefeitura.\n", nome, sobrenome);
    }
    else{
        printf("%s %s recebeu %d doce(s) das familias.\n", nome, sobrenome, doces);
    }
}

int main(){
    char linha_atual[80];
    char comando[30];
    char nome[20];
    int doces;
    p_hash hash = nasce_o_hash();
    while (1){
        fgets(linha_atual, 40, stdin);
        sscanf(linha_atual, "%s ", comando); // lê a primeira string da linha (comando)
        if (!strcmp(comando, "ENTRAR")){
            p_familia fam;
            sscanf(linha_atual, "%s %s %d", comando, nome, &doces);
            fam = nasce_a_familia(nome, doces);
            insere(hash, fam);
        }
        else if(!strcmp(comando, "COMPRAR")){
            sscanf(linha_atual, "%s %s %d", comando, nome, &doces);
            comprar(hash, nome, doces);
        }
        else if (!strcmp(comando, "SAIR")){
            sscanf(linha_atual, "%s %s", comando, nome);
            remover(hash, nome);
        }
        else if (!strcmp(comando, "FINALIZAR")){
            break;
        }
        else if (!strcmp(comando, "TRICKORTREAT")){
            char nome_crianca[20];
            char sobrenome_crianca[20];
            int qtd_casas = 0;
            sscanf(linha_atual, "%s %s %s %d", comando, nome_crianca, sobrenome_crianca, &qtd_casas);
            trick_or_treat(hash, nome_crianca, sobrenome_crianca, qtd_casas);
        }
    }
    libera_hash(hash);
    return 0;
}
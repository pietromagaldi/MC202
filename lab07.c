#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// def peca

typedef struct peca *p_peca;

struct peca{
    int tipo;
    char tamanho[8];
    int tempo_class, tempo_emb;
};

// funcoes peca

p_peca calcula_tempo(p_peca peca){
    if (peca->tamanho[0] == 'p'){ // se a peça for pequena
        peca->tempo_class = 1;
        peca->tempo_emb = peca->tipo;
    }
    else{
        peca->tempo_emb = (peca->tipo)+1;
        if (peca->tamanho[0] == 'm'){ // se a peça for média
            peca->tempo_class = 2;
        }
        else if (peca->tamanho[0] == 'g'){ // se a peça for grande
            peca->tempo_class = 3;
        }
    }
    return peca;
}

p_peca cria_peca(int tipo, char tamanho[8]){
    p_peca peca;
    peca = malloc(sizeof(struct peca));
    strcpy(peca->tamanho, tamanho);
    peca->tipo = tipo;
    peca = calcula_tempo(peca);
    return peca;
}

void libera_peca(p_peca peca){
    free(peca);
}

// def trabalhador

typedef struct trabalhador *p_trabalhador;

struct trabalhador{
    int tempo_com_peca;
    int ocupado;
    char tipo[14];
    p_peca peca_atual;
};

// funcoes trabalhador

p_trabalhador cria_trabalhador(char tipo[10]){
    p_trabalhador proletario;
    proletario = malloc(sizeof(struct trabalhador));
    strcpy(proletario->tipo, tipo);
    proletario->peca_atual = NULL; // começa sem nenhuma peça na mão
    proletario->tempo_com_peca = 0;
    proletario->ocupado = 0;
    return proletario;
}

void libera_trabalhador(p_trabalhador trabalhador){
    free(trabalhador);
}

// def lista ligada

typedef struct no *p_no;

struct no {
    p_peca dado;
    p_no prox;
};

// def pilha

typedef struct pilha *p_pilha;

struct pilha{
    int tamanho_limite;
    int tamanho_atual;
    int saiu, entrou; // usado no print minuto a minuto (podem ser 1 ou 0)
    p_no topo;
};

// funcoes pilha 

p_pilha cria_pilha(int tam_limite){
    p_pilha pilha;
    pilha = malloc(sizeof(struct pilha));
    pilha->tamanho_atual = 0;
    pilha->tamanho_limite = tam_limite;
    pilha->topo = NULL;
    pilha->entrou = 0;
    pilha->saiu = 0;
    return pilha;
}

void empilhar(p_pilha pilha, p_peca dado){
    p_no novo = malloc(sizeof(struct no));
    novo->dado = dado;
    novo->prox = pilha->topo;
    pilha->topo = novo;
    (pilha->tamanho_atual)++;
}

p_peca desempilhar(p_pilha pilha){
    p_no aux = pilha->topo;
    if (!aux){
        return NULL; // retorna null se não tiver nenhum elemento na pilha
    }
    p_peca dado = aux->dado;
    if (pilha->topo->prox == NULL){
        pilha->topo = NULL;
    }
    else{
        pilha->topo = pilha->topo->prox;
    }
    free(aux);
    (pilha->tamanho_atual)--;
    return dado;
}

void libera_pilha(p_pilha pilha){
    free(pilha); // só precisamos dar free na pilha, uma vez que no 'desempilhar' já damos free nos nós
}

// def fila

typedef struct fila *p_fila;

struct fila{
    p_peca *dados;
    int tamanho_limite, tamanho_atual;
    int ini, fim; // indices
    int entrou, saiu; 
};

// funcoes fila

p_fila cria_fila(int tam_limite){
    p_fila fila;
    fila = malloc(sizeof(struct fila));
    fila->dados = malloc((tam_limite + 1) * sizeof(struct peca));
    fila->tamanho_limite = tam_limite;
    fila->tamanho_atual = 0;
    fila->ini = 0;
    fila->fim = 0;
    fila->entrou = 0;
    fila->saiu = 0;
    return fila;
}

void enfileira(p_fila fila, p_peca dado){
    fila->dados[fila->fim] = dado;
    (fila->fim)++;
    (fila->tamanho_atual)++;
}

p_peca desenfileira(p_fila fila){
    p_peca dado = fila->dados[fila->ini];
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

p_fila le_pecas(p_fila esteira_reserva, int n_pecas){
    int tipo;
    char tamanho[8];
    p_peca peca;
    for (int i=0; i < n_pecas; i++){
        scanf("%s %d ", tamanho, &tipo);
        peca = cria_peca(tipo, tamanho);
        enfileira(esteira_reserva, peca);
    }
    return esteira_reserva;
}

// main

int main(){
    int tam_esteira, tam_pilha, n_pecas, minuto_atual = 0, atraso = 0; 
    p_peca peca;
    p_fila esteira, esteira_reserva;
    p_pilha pilha;
    p_trabalhador embalador, classificador;

    scanf("%d %d ", &tam_esteira, &tam_pilha);
    pilha = cria_pilha(tam_pilha);

    esteira = cria_fila(tam_esteira);
    scanf("%d ", &n_pecas);
    esteira_reserva = cria_fila(n_pecas);

    embalador = cria_trabalhador("embalador");
    classificador = cria_trabalhador("classificador");

    esteira_reserva = le_pecas(esteira_reserva, n_pecas); // primeiro, lemos e criamos todas as peças, e armazenamos elas na esteira reserva
    // depois desenfileiramos as peças da esteira reserva para a esteira principal, ou seja, a que possui um tamanho máximo

    printf("##### Simulação\n");

    while (1){
        minuto_atual++;
        if (embalador->peca_atual){ // se o embalador possuir uma peça na mão
            (embalador->tempo_com_peca)++;
            if (embalador->tempo_com_peca == embalador->peca_atual->tempo_emb){ // se ele tiver terminado de embalar a peça
                libera_peca(embalador->peca_atual);
                embalador->peca_atual = desempilhar(pilha); // pega, no mesmo minuto, uma outra peça da pilha
                pilha->saiu = 1;
                embalador->tempo_com_peca = 0; // reseta o tempo que o embalador ficou com a nova peça
            }
        }

        else if (pilha->tamanho_atual > 0){ // se há uma peça na pilha
            embalador->peca_atual = desempilhar(pilha); // nesse caso, caso o embalador não possua uma peça, ele a pega da pilha
        }


        if (classificador->ocupado){ // se o classificador possuir uma peça na mão
            (classificador->tempo_com_peca)++;
            if (classificador->tempo_com_peca == classificador->peca_atual->tempo_class){ // se ele tiver terminado de classificar a peça
                if (pilha->tamanho_atual < pilha->tamanho_limite){ // se ainda cabem peças na pilha
                    empilhar(pilha, classificador->peca_atual);
                    if (esteira->tamanho_atual > 0){
                        classificador->peca_atual = desenfileira(esteira);
                        classificador->ocupado = 1;
                    }
                    else{
                        classificador->peca_atual = NULL;
                        classificador->ocupado = 0;
                    }
                    classificador->tempo_com_peca = 0; // reseta o tempo que o classificador ficou com a nova peça
                    pilha->entrou = 1;
                    esteira->saiu = 1;
                }
                else{
                    (classificador->tempo_com_peca)--; // caso a pilha esteja cheia, o classificador segura a peça na mão, até que consiga empilhar
                }
            }
        }

        else if(esteira->tamanho_atual > 0){ // se houver peças na esteira
            classificador->peca_atual = desenfileira(esteira);
            classificador->ocupado = 1;
        }

        else{
            classificador->peca_atual = NULL; // classificador não possui peça na mão
            classificador->ocupado = 0;
        }

        if (esteira->tamanho_atual == esteira->tamanho_limite && esteira_reserva->tamanho_atual > 0){
            atraso++; // quando ainda há peças a serem enfileiradas na esteira principal, mas ela está cheia
        }
        else if (esteira->tamanho_atual < esteira->tamanho_limite && esteira_reserva->tamanho_atual > 0){ // cabem peças na esteira principal
            peca = desenfileira(esteira_reserva);
            enfileira(esteira, peca);
            esteira->entrou = 1;
        }

        printf("Min. %d: Classificação (+%d,-%d) | Embalagem (+%d,-%d).\n", minuto_atual, esteira->entrou, esteira->saiu, pilha->entrou, pilha->saiu);

        pilha->saiu = 0; // resetar as entradas/saídas de peças
        pilha->entrou = 0;
        esteira->entrou = 0;
        esteira->saiu = 0;

        if (pilha->tamanho_atual == 0 && esteira->tamanho_atual == 0 && // critério de parada (tá tudo vazio)
        esteira_reserva->tamanho_atual == 0 && !classificador->ocupado && !embalador->peca_atual){
            libera_fila(esteira);
            libera_fila(esteira_reserva);
            libera_trabalhador(classificador); // tá todo mundo livre
            libera_trabalhador(embalador);
            libera_pilha(pilha);
            break;
        }
    }
    printf("Tempo atraso: %d.", atraso);
    return 0;
}
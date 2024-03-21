#include <stdio.h>
#include <stdlib.h>

typedef struct vetor *p_vetor;

struct vetor{
    int *dados;
    int alocado;
    int utilizado;
};

p_vetor batalloc_inicial(){
    p_vetor batmemoria;
    batmemoria = malloc(sizeof(struct vetor));
    batmemoria->dados = malloc(8*sizeof(int));
    batmemoria->alocado = 8;
    batmemoria->utilizado = 0;
    for (int i=0; i<8; i++){
        batmemoria->dados[i] = -99;
    }
    return batmemoria;
}

void altera_tamanho(p_vetor batmemoria, int param){
    int *temp;
    temp = batmemoria->dados; // temp armazena os dados da batmemoria
    int a = batmemoria->alocado;
    if (param == 2){
        batmemoria->alocado *= param; 
    }
    else if (param == 9){
        batmemoria->alocado = batmemoria->alocado/2;
    }
    batmemoria->dados = malloc((batmemoria->alocado) * sizeof(int)); // dobra o tamanho da batmemoria
    for (int i=0; i<batmemoria->alocado; i++){
        if (i>=a){
        batmemoria->dados[i] = -99; // transfere os dados antigos da batmemoria pra nova
        }
        else{
        batmemoria->dados[i] = temp[i]; // preenche todos os espacos nao usados com -99
        }
    }
    free(temp); // libera temp
}

int onde_cabe(p_vetor batmemoria, int tam){
    int indice = 0;
    for (int i=0; i<(batmemoria->alocado); i++){
        if (batmemoria->dados[i] < 0){ // como os espacos nao usados sao preenchidos com -99, isso detecta espacos 'vazios'
            if (i == 0 || batmemoria->dados[i-1] > 0){ // quando i=0 ou quando i é a primeira casa vazia (a anterior estava ocupada)
                indice = i; 
            }
            else if (tam == i-indice){
                break;
            }
            if (i+1 == batmemoria->alocado){
                altera_tamanho(batmemoria, 2);
                break;
        }
        }
        else if (i+1 == batmemoria->alocado){
            altera_tamanho(batmemoria, 2);
            indice = i+1;
            break;
        }
    }
    return indice;
}

void adicionar_elemento(p_vetor batmemoria, int x, int indice){
    (batmemoria->utilizado)++;
    batmemoria->dados[indice] = x;
}

int indice_ultimo_valor(p_vetor batmemoria){
    int indice=0;
    for (int i=0; i < (batmemoria->alocado) ; i++){
        if (batmemoria->dados[i] > 0){
            indice = i;
        }
    }
    return indice;
}

void batfree(int endereco, p_vetor batmemoria){
    int ind =0;
    int aux = batmemoria->dados[endereco];
    for (int i=0; i <= aux ; i++){
        batmemoria->dados[endereco+i] = -99; // preenche os espacos nao usados com -99
        (batmemoria->utilizado)--;
    }
    ind = indice_ultimo_valor(batmemoria);
    if (ind < (batmemoria->alocado)/4){
        altera_tamanho(batmemoria, (9));
    }
}

void batprint(p_vetor batmemoria, int endereco){ // printa certo conjunto de numeros da batmemoria
    for (int i=1; i <= (batmemoria->dados[endereco]) ; i++){
        printf("%d ", batmemoria->dados[endereco+i]);
    }
    printf("\n");
}

void batuso(p_vetor batmemoria){ // printa quanto espaco foi utilizado do total
    printf("%d de %d\n", batmemoria->utilizado, batmemoria->alocado);
}

int main(){
    int n_comandos;
    p_vetor bat_memoria;
    scanf("%d", &n_comandos);
    bat_memoria = batalloc_inicial();
    for (int i=0; i<n_comandos; i++){
        char comando[10];
        scanf("%s", comando);
        if (comando[4] == 'a'){ // se o comando for bat-alloc
            int tam=0, x=0;
            scanf("%d ", &tam);
            int a = onde_cabe(bat_memoria, tam);
            adicionar_elemento(bat_memoria, tam, a);
            for (int j=1; j<=tam; j++){
                scanf("%d ", &x);
                adicionar_elemento(bat_memoria, x, (a+j));
            }
            printf("%d\n", a);
        }
        else if (comando[4] == 'f'){ // se o comando for bat-free
            int endereco = 0;
            scanf("%d ", &endereco);
            batfree(endereco, bat_memoria);
        }
        else if (comando[4] == 'u'){ // se o comando for bat-uso
            batuso(bat_memoria);
        }
        else if(comando[4] == 'p'){ // se o comando for bat-print
            int endereco = 0;
            scanf("%d ", &endereco);
            batprint(bat_memoria, endereco);
        }
    }
    free(bat_memoria->dados);
    free(bat_memoria);
    return 0;
}
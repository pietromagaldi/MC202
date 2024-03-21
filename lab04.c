#include <stdio.h>
#include <stdlib.h>

void aloca_e_le(int ***matriz, int tam){ // aloca e lê a matriz
    *matriz = malloc(tam*sizeof(int *));
    for (int i=0; i<tam; i++){
        (*matriz)[i] = malloc(tam*sizeof(int));
        for (int j=0; j<tam; j++){
            scanf("%d ", &(*matriz)[i][j]);
        }
    }
}

void printa(int **matriz, int tam, int ver){ // imprime a matriz
    if (ver == 0){
        printf("Quadras:\n");
    }
    else{
        printf("Conjuntos que precisam de atenção:\n");
    }
    for (int i=0; i<tam; i++){
        for (int j=0; j<tam; j++){
            printf("%d", (matriz[i])[j]);
            if (j == tam-1){
                printf("\n");
            }
            else{
                printf("\t");
            }
        }
    }
    printf("\n");
}

// dividimos a matriz original em 4 submatrizes, a sup esquerda, sup direita, inf esquerda e inf direita

int soma_sup_esq(int **matriz, int tam){ // encontra a soma da submatriz sup esq
    int soma=0;
    for (int i=0; i<tam-1; i++){
        for (int j=0; j<tam-1; j++){
            soma += (matriz[i])[j];
        }
    }
    return soma;
}

int soma_sup_dir(int **matriz, int tam){ // encontra a soma da submatriz sup dir
    int soma=0;
    for (int i=0; i<tam-1; i++){
        for (int j=1; j<tam; j++){
            soma += (matriz[i])[j];
        }
    }
    return soma;
}

int soma_inf_esq(int **matriz, int tam){ // encontra a soma da submatriz inf esq
    int soma=0;
    for (int i=1; i<tam; i++){
        for (int j=0; j<tam-1; j++){
            soma += (matriz[i])[j];
        }
    }
    return soma;
}

int soma_inf_dir(int **matriz, int tam){ // encontra a soma da submatriz inf dir
    int soma=0;
    for (int i=1; i<tam; i++){
        for (int j=1; j<tam; j++){
            soma += (matriz[i])[j];
        }
    }
    return soma;
}

int acha_o_max(int lista[3]){ // acha a maior soma
    int indice_max = 0, max = lista[0];
    for (int i=1; i<4; i++){
        if (lista[i]>max){
            max = lista[i];
            indice_max = i;
        }
    }
    return indice_max;
}

void libera(int ***matriz, int *tam){ // libera a matriz
    for (int i=0; i<(*tam); i++){
        free((*matriz)[i]);
    }
    free(*matriz);
}

void muda_a_matriz(int ***matriz, int *tam, int somas[4]){ // transforma a matriz original na submatriz desejada
    if (acha_o_max(somas) == 0){
        (*tam)--;
        int **nova_matriz;
        nova_matriz = malloc((*tam)*sizeof(int *));
        for (int i=0; i<*tam; i++){
            nova_matriz[i] = malloc((*tam)*sizeof(int));
            for (int j=0; j<*tam; j++){
                (nova_matriz[i])[j] = (*matriz)[i][j];
            }
        }
        int a = *tam;
        a++;
        libera(matriz, &a);
        *matriz = nova_matriz; // alocamos uma nova matriz, copiamos os elementos desejados e demos liberamos a matriz antiga
        }
    else if (acha_o_max(somas) == 1){
        (*tam)--;
            int **nova_matriz;
        nova_matriz = malloc((*tam)*sizeof(int *));
        for (int i=0; i<*tam; i++){
            nova_matriz[i] = malloc((*tam)*sizeof(int));
            for (int j=1; j<(*tam)+1; j++){
                (nova_matriz[i])[j-1] = ((*matriz)[i])[j];
            }
        }
        int a=*tam;
        a++;
        libera(matriz, &a);
        *matriz = nova_matriz;
    }
    else if (acha_o_max(somas) == 2){
        (*tam)--;
        int **nova_matriz;
        nova_matriz = malloc((*tam)*sizeof(int *));
        for (int i=1; i<(*tam)+1; i++){
            nova_matriz[i-1] = malloc((*tam)*sizeof(int));
            for (int j=0; j<*tam; j++){
                (nova_matriz[i-1])[j] = ((*matriz)[i])[j];
            }
        } 
        int a=*tam;
        a++;
        libera(matriz, &a);
        *matriz = nova_matriz;
    }
    else if (acha_o_max(somas) == 3){
        (*tam)--;
        int **nova_matriz;
        nova_matriz = malloc((*tam)*sizeof(int *));
        for (int i=1; i<(*tam)+1; i++){
            nova_matriz[i-1] = malloc((*tam)*sizeof(int));
            for (int j=1; j<(*tam)+1; j++){
                (nova_matriz[i-1])[j-1] = ((*matriz)[i])[j];
            }
        }
        int a=*tam;
        a++;
        libera(matriz, &a);
        *matriz = nova_matriz;
    }
}

int main(){
    int tam, alt;
    scanf("%d ", &tam);
    int **matriz;
    aloca_e_le(&matriz, tam);
    printa(matriz, tam, 0);
    int somas[4];
    while (tam>1){
        somas[0] = soma_sup_esq(matriz, tam);
        somas[1] = soma_sup_dir(matriz, tam);
        somas[2] = soma_inf_esq(matriz, tam);
        somas[3] = soma_inf_dir(matriz, tam);
        muda_a_matriz(&matriz, &tam, somas);
        printa(matriz, tam, 1);
        alt = 0;
        if (tam != 1){
            scanf("%d ", &alt);   
        }
        int l, c;
        if (alt == 1){
            scanf("%d %d ", &l, &c);
            scanf("%d ", &matriz[l][c]); // altera um elemento na matriz
        }
        else{
            break; // acaba o programa caso alt!=1
        }
    }
    libera(&matriz, &tam);
    return 0; 
}
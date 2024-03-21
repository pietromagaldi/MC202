#include <stdio.h>
#define MAX 10000

int quant(int lista[], int tamanho, int numero){
    int i, n = 0;
    for (i=0; i<tamanho; i++) {
        if (lista[i] == numero) {
            n++;
        }
    }return n;
}


void maior_sequencia(int lista[], int tamanho){
    int maior_seq = 1, seq_atual = 1, n_maior_seq = lista[0];
    int i;
    for (i=1; i<tamanho; i++){
        if (lista[i] == lista[i-1]){
            seq_atual++;
            if (seq_atual>maior_seq){
                maior_seq = seq_atual;
                n_maior_seq = lista[i];
                }
            } else{
                seq_atual = 1;
            }
        } 
    printf("James vendeu %d saladas de %d reais consecutivamente", maior_seq, n_maior_seq);
}

int main(){
    int i, n_vendas = 0, qt_5, qt_7, qt_10;
    scanf("%d", &n_vendas);
    int lista_vendas[MAX];
    for (i=0; i<n_vendas; i++) {
        scanf("%d", &lista_vendas[i]);
    }
    qt_5 = quant(lista_vendas, n_vendas, 5);
    qt_7 = quant(lista_vendas, n_vendas, 7);
    qt_10 = quant(lista_vendas, n_vendas, 10);
    printf("James vendeu %d saladas de 5 reais, %d de 7 reais e %d de 10 reais\n", qt_5, qt_7, qt_10);
    maior_sequencia(lista_vendas, n_vendas);
}
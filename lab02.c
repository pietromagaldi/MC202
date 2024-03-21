#include <stdio.h>
#define MAX 300

int ve_tamanho(char palavra[MAX]){
    int contador = 0;
    for (; palavra[contador]!='\0'; contador++);
    return contador;
}

void ta_ou_nao(char mapa[][MAX], char palavra[MAX], int n_linhas, int n_colunas){
    int contador = 1, linha = 0, coluna = 0;
    int verificador = 0; //0 quer dizer que a palavra não está no texto
    int tamanho_palavra = ve_tamanho(palavra);
    for (linha = 0; linha < n_linhas; linha++){
        for (coluna = 0; coluna < n_colunas; coluna++){
            if (mapa[linha][coluna] == palavra[0]){
                if (linha+1 - tamanho_palavra >= 0){
                    //verifica pra cima;
                    for (contador=1; contador<tamanho_palavra; contador++){
                        if (mapa[linha-contador][coluna] != palavra[contador]){
                            break;
                        }
                        else if (contador+1 == tamanho_palavra){
                            verificador = 1;
                        }
                    }
                }
                if (linha + tamanho_palavra <= n_linhas){
                    //verifica pra baixo;
                    for (contador=1; contador<tamanho_palavra; contador++){
                        if (mapa[linha+contador][coluna] != palavra[contador]){
                            break;
                        }
                        else if (contador+1 == tamanho_palavra){
                            verificador = 1;
                        }
                    }
                }
                if (coluna + tamanho_palavra <= n_colunas){
                    //verifica pra direita;
                    for (contador=1; contador<tamanho_palavra; contador++){
                        if (mapa[linha][coluna+contador] != palavra[contador]){
                            break;
                        }
                        else if (contador+1 == tamanho_palavra){
                            verificador = 1;
                        }
                    }
                }
                if (coluna+1 - tamanho_palavra >= 0){
                    //verifica pra esquerda;
                    for (contador=1; contador<tamanho_palavra; contador++){
                        if (mapa[linha][coluna-contador] != palavra[contador]){
                            break;
                        }
                        else if (contador+1 == tamanho_palavra){
                            verificador = 1;
                        }
                    }
                }   
            }
        }     
    }
    if (verificador != 0){
        printf("A palavra %s está no texto!\n", palavra);
    }
    else{
        printf("A palavra %s não está no texto!\n", palavra);
    }
}

void cria_mapa(char matriz[][MAX], int n_linhas, int n_colunas){
    for (int linha = 0; linha < n_linhas; linha++){
        for (int coluna = 0; coluna < n_colunas; coluna++){
            scanf(" %c ", &matriz[linha][coluna]);   
        }    
    }
}

int main(){
    int linhas, colunas, n_palavras;
    scanf("%d %d %d ", &linhas, &colunas, &n_palavras);
    char mapa[MAX][MAX], lista_palavras[MAX][MAX];
    cria_mapa(mapa, linhas, colunas);
    for (int i = 0; i < n_palavras; i++){
        scanf(" %s ", lista_palavras[i]);
    }
    for (int i=0; i<n_palavras; i++){
        ta_ou_nao(mapa, lista_palavras[i], linhas, colunas);
    }
    return 0;
}
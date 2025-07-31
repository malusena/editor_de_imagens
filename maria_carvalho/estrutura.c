#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estrutura.h"

// Aloca memoria para os ponteiros das linhas da matriz
pixel** alocarmatriz(pixel **matriz, int nlinhas, int mcolunas){
  matriz = (pixel **) calloc(nlinhas, sizeof(pixel*));
  if(matriz == NULL){
    perror("Erro para alocar os ponteiros para as linhas da matriz");
    return NULL;
  }
  // Para cada ponteiro da linha, aloca um array de pixels
  for(int i = 0; i < nlinhas; i++){
    matriz[i] = (pixel *) calloc(mcolunas, sizeof(pixel));
    if(matriz[i] == NULL){
      perror("Erro para alocar memoria para uma linha da matriz");
      for(int j = 0; j < i; j++){
        free(matriz[j]);
      }
      free(matriz);
      return NULL;
    }
  }
  return matriz;
}

// Escreve a matriz de pixels no arquivo
void imprimirmatriz(pixel **matriz, const char *magicn, int largura, int altura, int maxval, const char *arqsaida){
  FILE *saida = NULL;
                                     
  if(strcmp(magicn, "P4") == 0 || strcmp(magicn, "P5") == 0 || strcmp(magicn, "P6") == 0) {
      saida = fopen(arqsaida, "wb");
  } else {
      saida = fopen(arqsaida, "w");
  }

  if(saida == NULL){
    perror("Erro para abrir o arquivo de saida, que sera a imagem editada");
    return;
  }

  // Escreve o cabeçalho
  if(strcmp(magicn, "P1") == 0 || strcmp(magicn, "P4") == 0) {
      fprintf(saida, "%s\n%d %d\n", magicn, largura, altura);
  } else {
      fprintf(saida, "%s\n%d %d\n%d\n", magicn, largura, altura, maxval);
  }

  // Escreve os dados dos pixels
  if(strcmp(magicn, "P1") == 0){
    for(int i = 0; i < altura; i++){
      for(int j = 0; j < largura; j++){
        fprintf(saida, "%d%s", (matriz[i][j].r == 0 ? 1 : 0), (j == largura - 1 ? "" : " "));
      }
      fprintf(saida, "\n");
    }
  } else if(strcmp(magicn, "P2") == 0){
    for(int i = 0; i < altura; i++){
      for(int j = 0; j < largura; j++){
        fprintf(saida, "%d%s", matriz[i][j].r, (j == largura - 1 ? "" : " "));
      }
      fprintf(saida, "\n");
    }
  } else if(strcmp(magicn, "P3") == 0){
    for(int i = 0; i < altura; i++){
      for(int j = 0; j < largura; j++){
        fprintf(saida, "%d %d %d%s", matriz[i][j].r, matriz[i][j].g, matriz[i][j].b, (j == largura - 1 ? "" : " "));
      }
      fprintf(saida, "\n");
    }
  } else if(strcmp(magicn, "P4") == 0){
    for(int i = 0; i < altura; i++){
      unsigned char byteval = 0;
      for(int j = 0; j < largura; j++){
        int bit = j % 8;
        if (matriz[i][j].r == 0) {
            byteval |= (1 << (7 - bit));
        }
        if (bit == 7 || j == largura - 1){
          if(fwrite(&byteval, sizeof(unsigned char), 1, saida) != 1){
            fprintf(stderr, "Erro para escrever os dados\n");
            fclose(saida);
            return;
          }
          byteval = 0;
        }
      }
    }
  } else if(strcmp(magicn, "P5") == 0){
    for(int i = 0; i < altura; i++){
      for(int j = 0; j < largura; j++){
        if(fwrite(&matriz[i][j].r, sizeof(unsigned char), 1, saida) != 1){
          fprintf(stderr, "Erro para escrever os dados\n");
          fclose(saida);
          return;
        }
      }
    }
  } else if(strcmp(magicn, "P6") == 0){
    for(int i = 0; i < altura; i++){
      if(fwrite(matriz[i], sizeof(pixel), largura, saida) != largura){
        fprintf(stderr, "Erro para escrever os dados nas linhas do arquivo\n");
        break;
      }
    }
  } else{
      fprintf(stderr, "Erro, o magic number e invalido\n");
  }

  fclose(saida);
}

// Libera a memoria alocada para a matriz de pixels
void liberarmatriz(pixel **matriz, int nlinhas){
  if(matriz == NULL){
    return;
  }
  for(int i = 0; i < nlinhas; i++){
    free(matriz[i]);
  }
  free(matriz);
}
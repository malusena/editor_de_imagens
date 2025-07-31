#ifndef ESTRUTURA_H
#define ESTRUTURA_H

// Define a cor do pixel
typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} pixel;

// Funcoes
pixel** alocarmatriz(pixel **matriz, int nlinhas, int mcolunas);
void imprimirmatriz(pixel **matriz, const char *magicn, int largura, int altura, int maxval, const char *arqsaida);
void liberarmatriz(pixel **matriz, int nlinhas);

#endif
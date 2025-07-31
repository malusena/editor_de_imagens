#ifndef IMAGENS_PPM_H
#define IMAGENS_PPM_H
#include "estrutura.h"

//Funcoes
void ampliaimagem(const char *arqentrada, const char *arqsaida, int fator);
void reduzimagem(const char *arqentrada, const char *arqsaida, int fator);
void rotaciona(const char *arqentrada, const char *arqsaida, int angulo);
void converteparacinza(const char *arqentrada, const char *arqsaida);
void invertecores(const char *arqentrada, const char *arqsaida);

pixel **leimagempfm(const char *arqentrada, char *magicn, int *largura, int *altura, int *maxval);
unsigned char calculatoncinza(unsigned char r, unsigned char g, unsigned char b);

#endif
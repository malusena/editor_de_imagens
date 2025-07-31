#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "imagens_ppm.h"
#include "estrutura.h"

// Menu principal
void menu(const char *nomeprograma){
    printf("Funcionalidades:\n");
    printf("\n");

    printf("1 - Redimensionar - Reduzir\n");
    printf("Digite reduzir\n");
    printf("Para o fator, escolha um numero de 1 a 4\n");
    printf("\n");

    printf("2 - Redimensionar - Ampliar\n");
    printf("Digite ampliar\n");
    printf("Para o fator, escolha um numero de 1 a 4\n");
    printf("\n");

    printf("3 - Rotacionar imagem\n");
    printf("Digite rotacionar\n");
    printf("Para o angulo, escolha 90, 180 ou 270 graus\n");
    printf("\n");

    printf("4 - Converter para escala de cinza\n");
    printf("Digite cinza\n");
    printf("\n");

    printf("5. Inverter Cores Negativo\n");
    printf("Digite negativo\n");
    printf("\n");

    printf("\nExemplo, %s imagem.ppm reduzir 2 imagemreduzida.ppm\n", nomeprograma);
    printf("\n");
    printf("Esse e padrao de comando para alterar sua imagem\n");
    printf("./%s suaimagem.ppm funcionalidade parametro suaimagemsaida.ppm\n", nomeprograma);
}


int main(int argc, char *argv[]){
// Verifica se o usuario deu todos os argumentos para modificar a imagem
 if(argc < 4){
  fprintf(stderr, "Erro, falta o argumento\n");
  menu(argv[0]);
  return 1;
 }

 const char *arqentrada = argv[1];
 const char *arqsaida = argv[argc - 1];

 //Verifica qual foi a funcao foi chamada pelo terminal
 const char *filtroescolhido = argv[2];

//Verifica se foi dado um valor para a funcionalidade
 if((strcmp(filtroescolhido, "reduzir")) == 0){
  if(argc < 5){
   fprintf(stderr, "Erro, falta o valor de reducao\n");
   return 1;
  }
  int fator = atoi(argv[3]);
  if(fator < 1 || fator > 4){
   fprintf(stderr, "Erro, o valor de reducao e invalido, escolha um numero de 1 a 4\n");
   return 1;
  }
  reduzimagem(arqentrada, arqsaida, fator);
 }
 else if((strcmp(filtroescolhido, "ampliar")) == 0){
  if(argc < 5){
   fprintf(stderr, "Erro, falta o valor de ampliacao\n");
   return 1;
  }
  int fator = atoi(argv[3]);
  if(fator < 1 || fator > 4){
   fprintf(stderr, "Erro, o valor de ampliacao e invalido, escolha um numero de 1 a 4\n");
   return 1;
  }
  ampliaimagem(arqentrada, arqsaida, fator);
 }
else if((strcmp(filtroescolhido, "rotacionar")) == 0){
  if(argc < 5){
    fprintf(stderr, "Erro, falta o valor de rotacao\n");
    return 1;
   }
  int angulo = atoi(argv[3]);
  if(angulo != 90 && angulo != 180 && angulo != 270){
    fprintf(stderr, "Erro, angulo de rotacao invalido, escolha 90, 180 ou 270\n");
    return 1;
   }
  rotaciona(arqentrada, arqsaida, angulo);
 }
 else if((strcmp(filtroescolhido, "cinza")) == 0){
  if(argc != 4) {
   fprintf(stderr, "Erro, o cinza nao precisa de valor\n");
   return 1;
  }
  converteparacinza(arqentrada, arqsaida);
 }
 else if((strcmp(filtroescolhido, "negativo")) == 0){
  if(argc != 4) {
   fprintf(stderr, "Erro, o negativo nao precisa de valor\n");
   return 1;
  }
  invertecores(arqentrada, arqsaida);
 }
 else {
  fprintf(stderr, "Erro, essa opcao nao existe\n");
  return 1;
 }

 return 0;
}
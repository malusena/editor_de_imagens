#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estrutura.h"
#include "imagens_ppm.h"

// Le a imagem
// A funcao reune a abertura do arquivo, a leitura do cabeçalho do ppm e dos pixels
// Qual o plural de pixel? Pixels ou pixeis?
pixel **leimagempfm(const char *arqentrada, char *magicn, int *largura, int *altura, int *maxval){
  FILE *imagem = fopen(arqentrada, "rb");
  if(!imagem){
    perror("Erro ao abrir o arquivo de entrada, que e a imagem que sera editada");
    return NULL;
  }

  // Le o magic number
  if(fscanf(imagem, "%2s", magicn) != 1){
    fprintf(stderr, "Erro ao ler o magic number do arquivo\n");
    fclose(imagem);
    return NULL;
  }

  // Le o restante do arquivo apos o magic number
  char c;
  while((c = fgetc(imagem)) != '\n' && c != EOF);


  // Identifica e ignora os comentarios presentes no arquivo, se houver
  c = fgetc(imagem);
  while(c == '#'){
    while(c != '\n' && c != EOF){
      c = fgetc(imagem);
    }
    c = fgetc(imagem);
  }
  ungetc(c, imagem);

  // Le a largura, altura e valor maximo, se aplicavel
  if(fscanf(imagem, "%d %d", largura, altura) != 2){
    fprintf(stderr, "Erro ao ler a largura e altura do arquivo\n");
    fclose(imagem);
    return NULL;
  }

  if(strcmp(magicn, "P1") != 0 && strcmp(magicn, "P4") != 0){
    if(fscanf(imagem, "%d", maxval) != 1){
      fprintf(stderr, "Erro ao ler o valor maximo de cor do arquivo\n");
      fclose(imagem);
      return NULL;
    }
  } else {
      *maxval = 1;
  }

  // Tira o espaco que tem depois do valor maximo
  fgetc(imagem);

  // Validação do formato e maxval(conforme requisitos do trabalho)
  if(strcmp(magicn, "P1") != 0 && strcmp(magicn, "P2") != 0 &&
     strcmp(magicn, "P3") != 0 && strcmp(magicn, "P4") != 0 &&
     strcmp(magicn, "P5") != 0 && strcmp(magicn, "P6") != 0){
    fprintf(stderr, "Arquivo invalido\n");
    fclose(imagem);
    return NULL;
  }
  
  // Aloca memoria para a matriz de pixels em larguraxaltura
  pixel **imgdata = NULL;
  imgdata = alocarmatriz(imgdata, *altura, *largura);
  if(!imgdata){
    fprintf(stderr, "Erro ao alocar memoria para a imagem\n");
    fclose(imagem);
    return NULL;
  }

  // Leitura dos dados dos pixels dependendo do magic number
  // ppm do tipo P1
  if(strcmp(magicn, "P1") == 0){
    int pixelval;
    for(int i = 0; i < *altura; i++){
      for(int j = 0; j < *largura; j++){
        if(fscanf(imagem, "%d", &pixelval) != 1){
          fprintf(stderr, "Erro ao ler os dados dos pixels\n");
          liberarmatriz(imgdata, *altura);
          fclose(imagem);
          return NULL;
        }
        imgdata[i][j].r = imgdata[i][j].g = imgdata[i][j].b = (unsigned char)(pixelval == 1 ? 0 : 255);
      }
    }
    // ppm do tipo P2
  } else if(strcmp(magicn, "P2") == 0){
    int pixelval;
    for(int i = 0; i < *altura; i++){
      for(int j = 0; j < *largura; j++){
        if(fscanf(imagem, "%d", &pixelval) != 1){
          fprintf(stderr, "Erro ao ler os dados dos pixels\n");
          liberarmatriz(imgdata, *altura);
          fclose(imagem);
          return NULL;
        }
        unsigned char cinza = (unsigned char)(((float)pixelval / *maxval) * 255.0f);
        imgdata[i][j].r = imgdata[i][j].g = imgdata[i][j].b = cinza;
      }
    }
    // ppm do tipo P3
  } else if(strcmp(magicn, "P3") == 0){ 
    int r, g, b;
    for(int i = 0; i < *altura; i++){
      for(int j = 0; j < *largura; j++){
        if(fscanf(imagem, "%d %d %d", &r, &g, &b) != 3){
          fprintf(stderr, "Erro ao ler os dados dos pixels\n");
          liberarmatriz(imgdata, *altura);
          fclose(imagem);
          return NULL;
        }
        imgdata[i][j].r = (unsigned char)(((float)r / *maxval) * 255.0f);
        imgdata[i][j].g = (unsigned char)(((float)g / *maxval) * 255.0f);
        imgdata[i][j].b = (unsigned char)(((float)b / *maxval) * 255.0f);
      }
    }
    // ppm do tipo P4
  } else if(strcmp(magicn, "P4") == 0){ 
    int bytesporlinha = (*largura + 7) / 8;
    unsigned char byteval;
    for(int i = 0; i < *altura; i++){
      for(int j = 0; j < bytesporlinha; j++){
        if(fread(&byteval, sizeof(unsigned char), 1, imagem) != 1){
          fprintf(stderr, "Erro ao ler os dados dos pixels\n");
          liberarmatriz(imgdata, *altura);
          fclose(imagem);
          return NULL;
        }
        for(int bit = 0; bit < 8; bit++){
          if((j * 8 + bit) < *largura){
            unsigned char pixelcor = ((byteval >> (7 - bit)) & 1) == 1 ? 0 : 255;
            imgdata[i][j * 8 + bit].r = imgdata[i][j * 8 + bit].g = imgdata[i][j * 8 + bit].b = pixelcor;
          }
        }
      }
    }
    // ppm do tipo P5
  } else if(strcmp(magicn, "P5") == 0){ 
    unsigned char byteval;
    for(int i = 0; i < *altura; i++){
      for(int j = 0; j < *largura; j++){
        if(fread(&byteval, sizeof(unsigned char), 1, imagem) != 1){
          fprintf(stderr, "Erro ao ler os dados dos pixels\n");
          liberarmatriz(imgdata, *altura);
          fclose(imagem);
          return NULL;
        }
        unsigned char cinza = (unsigned char)(((float)byteval / *maxval) * 255.0f);
        imgdata[i][j].r = imgdata[i][j].g = imgdata[i][j].b = cinza;
      }
    }
    // ppm do tipo P6
  } else if(strcmp(magicn, "P6") == 0){
    for(int i = 0; i < *altura; i++){
      if(fread(imgdata[i], sizeof(pixel), *largura, imagem) != *largura){
        fprintf(stderr, "Erro ao ler os dados dos pixels da linha\n");
        liberarmatriz(imgdata, *altura);
        fclose(imagem);
        return NULL;
      }
    }
  }

  fclose(imagem);
  return imgdata;
}

// Redimensionamento - Ampliar
void ampliaimagem(const char *arquivoentrada, const char *arquivosaida, int fator){
  char numeromagico[3];
  int morig, norig, cores; // morig para largura, norig para altura
  pixel **imagemoriginal = leimagempfm(arquivoentrada, numeromagico, &morig, &norig, &cores);
  if(!imagemoriginal){
    return;
  }

  int nampliada = norig * fator;
  int mampliada = morig * fator;

  pixel **imagemampliada = alocarmatriz(NULL, nampliada, mampliada);
  if(!imagemampliada){
    fprintf(stderr, "Erro ao alocar a memoria para imagem ampliada\n");
    liberarmatriz(imagemoriginal, norig);
    return;
  }

  for(int i = 0; i < nampliada; i++){
    for(int j = 0; j < mampliada; j++){
      imagemampliada[i][j] = imagemoriginal[i / fator][j / fator];
    }
  }

  imprimirmatriz(imagemampliada, "P6", mampliada, nampliada, 255, arquivosaida);

  liberarmatriz(imagemampliada, nampliada);
  liberarmatriz(imagemoriginal, norig);
}

// Cinza
unsigned char calculatoncinza(unsigned char r, unsigned char g, unsigned char b){
  int media =(r + g + b) / 3;
  unsigned char tonscinza[10] ={0, 28, 54, 79, 105, 128, 169, 192, 211, 220};

  int menordiferenca = 256;
  unsigned char corcinzamaisproxima = 0;

  for(int i = 0; i < 10; i++){
    int diferenca = abs(tonscinza[i] - media);
    if(diferenca < menordiferenca){
      menordiferenca = diferenca;
      corcinzamaisproxima = tonscinza[i];
    }
  }
  return corcinzamaisproxima;
}

// Negativo
void invertecores(const char *arquivoentrada, const char *arquivosaida){
  char numeromagico[3];
  int m, n, cores;
  pixel **imagemoriginal = leimagempfm(arquivoentrada, numeromagico, &m, &n, &cores);
  if(!imagemoriginal){
    return;
  }

  pixel **imageminvertida = alocarmatriz(NULL, n, m);
  if(!imageminvertida){
    fprintf(stderr, "Erro ao alocar a memoria para imagem invertida\n");
    liberarmatriz(imagemoriginal, n);
    return;
  }
   
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      imageminvertida[i][j].r =(unsigned char)(255 - imagemoriginal[i][j].r);
      imageminvertida[i][j].g =(unsigned char)(255 - imagemoriginal[i][j].g);
      imageminvertida[i][j].b =(unsigned char)(255 - imagemoriginal[i][j].b);
    }
  }
 
  imprimirmatriz(imageminvertida, "P6", m, n, 255, arquivosaida);

  liberarmatriz(imageminvertida, n);
  liberarmatriz(imagemoriginal, n);
}

// Escala de cinza
void converteparacinza(const char *arquivoentrada, const char *arquivosaida){
  char numeromagico[3];
  int m, n, cores;
  pixel **imagemoriginal = leimagempfm(arquivoentrada, numeromagico, &m, &n, &cores);
  if(!imagemoriginal){
    return;
  }

  pixel **imagemcinza = alocarmatriz(NULL, n, m);
  if(!imagemcinza){
    fprintf(stderr, "Erro ao alocar a memoria para imagem em escala de cinza\n");
    liberarmatriz(imagemoriginal, n);
    return;
  }

  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      unsigned char tomcinza = calculatoncinza(
        imagemoriginal[i][j].r,
        imagemoriginal[i][j].g,
        imagemoriginal[i][j].b
      );
      imagemcinza[i][j].r = tomcinza;
      imagemcinza[i][j].g = tomcinza;
      imagemcinza[i][j].b = tomcinza;
    }
  }

  imprimirmatriz(imagemcinza, "P6", m, n, 255, arquivosaida);

  liberarmatriz(imagemcinza, n);
  liberarmatriz(imagemoriginal, n);
}

// Redimensionamento - Reduzir
void reduzimagem(const char *arquivoentrada, const char *arquivosaida, int fator){
  char numeromagico[3];
  int morig, norig, cores;
  pixel **imagemoriginal = leimagempfm(arquivoentrada, numeromagico, &morig, &norig, &cores);
  if(!imagemoriginal){
    return;
  }
  if(fator <= 0 ||(norig / fator == 0 && norig > 0) ||(morig / fator == 0 && morig > 0)){
    fprintf(stderr, "Erro, nao e possivel reduzir a imagem para resolucao zero\n");
    liberarmatriz(imagemoriginal, norig);
    return;
  }

  int nreduzida = norig / fator;
  int mreduzida = morig / fator;

  pixel **imagemreduzida = alocarmatriz(NULL, nreduzida, mreduzida);
  if(!imagemreduzida){
    fprintf(stderr, "Erro ao alocar memoria\n");
    liberarmatriz(imagemoriginal, norig);
    return;
  }

  for(int i = 0; i < nreduzida; i++){
    for(int j = 0; j < mreduzida; j++){
      imagemreduzida[i][j] = imagemoriginal[i * fator][j * fator];
    }
  }

  imprimirmatriz(imagemreduzida, "P6", mreduzida, nreduzida, 255, arquivosaida);

  liberarmatriz(imagemreduzida, nreduzida);
  liberarmatriz(imagemoriginal, norig);
}

// Rotacionar
void rotaciona(const char *arquivoentrada, const char *arquivosaida, int angulo){
  char numeromagico[3];
  int morig, norig, cores;
  pixel **imagemoriginal = leimagempfm(arquivoentrada, numeromagico, &morig, &norig, &cores);
  if(!imagemoriginal){
    return;
  }

  pixel **imagemrotacionada = NULL;
  int nrot, mrot;
  switch(angulo){
    case 90:
      nrot = morig;
      mrot = norig;
      imagemrotacionada = alocarmatriz(NULL, nrot, mrot);
      if(!imagemrotacionada){
        fprintf(stderr, "Erro ao alocar memoria\n");
        liberarmatriz(imagemoriginal, norig);
        return;
      }
      for(int i = 0; i < nrot; i++){
        for(int j = 0; j < mrot; j++){
          imagemrotacionada[i][j] = imagemoriginal[norig - 1 - j][i];
        }
      }
      break;
    case 180:
      nrot = norig;
      mrot = morig;
      imagemrotacionada = alocarmatriz(NULL, nrot, mrot);
      if(!imagemrotacionada){
        fprintf(stderr, "Erro ao alocar memoria\n");
        liberarmatriz(imagemoriginal, norig);
        return;
      }
      for(int i = 0; i < nrot; i++){
        for(int j = 0; j < mrot; j++){
          imagemrotacionada[i][j] = imagemoriginal[norig - 1 - i][morig - 1 - j];
        }
      }
      break;
    case 270:
      nrot = morig;
      mrot = norig;
      imagemrotacionada = alocarmatriz(NULL, nrot, mrot);
      if(!imagemrotacionada){
        fprintf(stderr, "Erro ao alocar memoria\n");
        liberarmatriz(imagemoriginal, norig);
        return;
      }
      for(int i = 0; i < nrot; i++){
        for(int j = 0; j < mrot; j++){
          imagemrotacionada[i][j] = imagemoriginal[j][morig - 1 - i];
        }
      }
      break;
    default:
      fprintf(stderr, "Erro, angulo de rotacao invalido, escolha 90, 180 ou 270\n");
      liberarmatriz(imagemoriginal, norig);
      return;
  }

  imprimirmatriz(imagemrotacionada, "P6", mrot, nrot, 255, arquivosaida);

  liberarmatriz(imagemrotacionada, nrot);
  liberarmatriz(imagemoriginal, norig);
}
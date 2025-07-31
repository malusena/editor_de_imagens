Este projeto é um sistema em C para manipulação básica de imagens no formato PPM (Portable Pixmap), o programa suporta todos os subformatos PPM para garantir uma maior compatibilidade. Ele se concentra na leitura, edição e salvamento das imagens editadas.

-----

## Funcionalidades para edição de imagem disponíveis no sistema:

* Redimensionamento - Reduzir: Diminui as dimensões da imagem por um fator específico (1 a 4), realizando uma amostragem simples para manter a proporção. O usuário escolhe um valor entre 1 a 4 para definir em quantas vezes ele vai reduzir a imagem.

* Redimensionamento - Ampliar: Aumenta as dimensões da imagem por um fator específico (1 a 4), replicando pixels para gerar a imagem ampliada e realizando uma amostragem simples para manter a proporção.O usuário escolhe um valor entre 1 a 4 para definir em quantas vezes ele vai ampliar a imagem.

* Rotação: Gira a imagem em ângulos predefinidos (90, 180 ou 270 graus), ajustando as dimensões conforme necessário.

* Conversão para Escala de Cinza: Transforma uma imagem colorida em uma imagem em tons de cinza, calculando a média dos componentes RGB de cada pixel e mapeando-a para uma paleta de 10 tons de cinza predefinidos.

* Inversão de Cores (Negativo): Aplica um efeito de negativo na imagem, subtraindo cada componente de cor de 255.

## Funcionalidades do sistema:

* Salvamento de Imagens:
Saída Padronizada: Salva todas as imagens processadas no formato PPM P6 (binário), garantindo compatibilidade e eficiência na escrita.

* Verificação de Argumentos: Garante que os argumentos necessários sejam fornecidos na linha de comando.

* Validação de Parâmetros de Filtro: Confere se os fatores de redimensionamento e os ângulos de rotação estão dentro dos limites permitidos, exibindo mensagens de erro claras se inválidos.

* Tratamento de Erros de Arquivo/Memória: Detecta e reporta falhas ao abrir arquivos ou alocar memória, prevenindo travamentos do programa.

## Estrutura do Projeto:

O projeto é modularizado para facilitar a organização e manutenção do código:

* main.c: Contém a lógica de execução principal do programa, o menu de opções e a interface para interpretar os comandos da linha de entrada do usuário.

* estrutura.h: Define a estrutura de dados pixel (que armazena os componentes RGB de uma cor) e declara os protótipos das funções essenciais para gerenciamento de memória de matrizes de pixels (alocarmatriz, liberarmatriz) e para a escrita da matriz de pixels em um arquivo (imprimirmatriz).

* estrutura.c: Implementa as funcionalidades de alocação, liberação e escrita de matrizes de pixels, suportando a escrita em todos os formatos PPM (P1 a P6).

* imagens_ppm.h: Atua como o arquivo de cabeçalho do módulo imagens_ppm. Ele declara os protótipos de todas as funções de processamento de imagem (ampliaimagem, reduzimagem, rotaciona, converteparacinza, invertecores) e das funções auxiliares de leitura (leimagempfm) e cálculo de tons de cinza (calculatoncinza).

* imagens_ppm.c: Contém a implementação detalhada de todos os filtros de imagem e da robusta função de leitura de imagens, que pode interpretar e carregar dados de qualquer tipo de arquivo PPM.

------

## Instruções para compilar:

É recomendado que seja executado em ambiente Linux. Para compilar, basta digitar "make" no terminal, no diretorio ../maria_carvalho/. Ele vai executar o comando gcc main.c estrutura.c imagens_ppm.c -o main e vai gerar um arquivo executável chamado main. Após isso, basta executar ./main no terminal.

Bash
gcc main.c estrutura.c imagens_ppm.c -o processador imagens

* Instruções para acessar o menu do programa:
Bash
./main 

* Instruções para editar uma imagem:
Bash
./main suaimagem.ppm funcionalidade parametro suaimagemsaida.ppm

* Exemplo:
Bash
./main imagem.ppm reduzir 2 imagemreduzida.ppm
./main imagem.ppm cinza imagemreduzida.ppm

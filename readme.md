# EP1 da disciplina de Introdução a Análise de Algoritmos da Escola de Artes Ciencias e Humanidades da USP

O exercicio tem como objetivo criar uma curva de Koch a partir de dois pontos, e de preencher o uma area com uma cor.

Para executar o programa é necessário primeiro compilar, para isso, com um compilador da linguagem C instalado em seu computador, digite no terminal:

```shell
gcc -Wall -o main main.c
```

Isso irá gerar o arquivo executável main.

Caso o aruivo não tenha permissão de execução digite:

```shell
sudo chmod +x ./main
```

Dessa forma o arquivo poderá ser executado.

## Arquivo de comandos

Para executar o programa é necessário um arquivo de comandos em txt que deve ter a seguinte estrutura:

```txt
largura altura cor_fundo
Comando argumentos
...
END
```

### Comandos e argumentos

1. SET_COLOR - 1 agumento
   1. Int - valor da cor
2. SET_PIXEL - 2 argumentos
   1. Int - Coordenada x
   2. Int - Coordenada y
3. DRAW_LINE - 4 argumentos
   1. Int - Coordenada x do primeiro ponto
   2. Int - Coordenada y do primeiro ponto
   3. Int - Coordenada x do segundo ponto
   4. Int - Coordenada y do segundo ponto
4. KOCH_CURVE - 5 argumentos
   1. Int - Coordenada x do primeiro ponto
   2. Int - Coordenada y do primeiro ponto
   3. Int - Coordenada x do segundo ponto
   4. Int - Coordenada y do segundo ponto
   5. Int - Valor do limiar
5. REGION_FILL - 2 argumentos
   1. Int - Coordenada x
   2. Int - Coordenada y

## Executando o programa

A forma de executar o programa é passando dois parametros, primeiro o arquivos de comandos e segundo o nome da imagem de saida .pgm, por exemplo:

```shell
./main comandos.txt imagem.pgm
```

E dessa forma o programa executará os comandos em sequencia do arquivo de comandos e gravará a imagem no arquivo de saida.

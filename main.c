////////////////////////////////////////////////////////////////
//                                                            //
// 9395067  - Vinicius Henrique Crispim Rosa                  //
// 11270653 - Marcelo Augusto Carvalho Meira Passos           //
//                                                            //
////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// constante que define o valor maximo que um pixel pode assumir.

#define MAX_VALUE 255

// constantes que definem as strings que podem ser especificadas no arquivo de entrada, cada uma associada um comando.

#define SET_COLOR "SET_COLOR"
#define SET_PIXEL "SET_PIXEL"
#define DRAW_LINE "DRAW_LINE"
#define KOCH_CURVE "KOCH_CURVE"
#define REGION_FILL "REGION_FILL"
#define END "END"

// estrutura que define o tipo Image (representa uma imagem digital).

typedef struct
{

  int width;    // largura da imagem (numero de colunas da imagem)
  int height;   // altura da imagem (numero de linhas da imagem)
  int **matrix; // ponteiro para a matriz que armazena o conteudo da imagem

} Image;

// estrutura que define o tipo Point (representa um ponto bidimensional).

typedef struct
{

  int x; // coordenada horizontal
  int y; // coordenada vertical

} Point;

// funcao que cria uma imagem vazia, com a cor de fundo especificada e a devolve.

Image *create_imagem(int w, int h, int bg_color)
{

  int lin, col;
  Image *image = (Image *)malloc(sizeof(Image));

  image->height = h;
  image->width = w;
  image->matrix = (int **)malloc(image->height * sizeof(int *));

  for (lin = 0; lin < image->height; lin++)
  {

    image->matrix[lin] = (int *)malloc(image->width * sizeof(int));

    for (col = 0; col < image->width; col++)
      image->matrix[lin][col] = bg_color;
  }

  return image;
}

// funcao que libera os recursos de memoria associados a uma imagem.

void free_image(Image *image)
{

  int lin;

  for (lin = 0; lin < image->height; lin++)
    free(image->matrix[lin]);

  free(image->matrix);
  free(image);
}

// funcao que salva uma imagem em um arquivo, no formato PGM. Programas/utilitarios que trabalham com
// imagens bitmap (como o Gimp e visualizadores de imagens normalmente instalados em ambiente Linux) em
// geral reconhecem este formato. Se tiver dificuldades para visualizar os arquivos de imagem salvos por
// esta funcao, um visualizador online pode ser encontrado neste link: http://paulcuth.me.uk/netpbm-viewer/.

void save_image(Image *image, char *file_name)
{

  int lin, col;
  FILE *out = fopen(file_name, "w");

  if (out)
  {

    fprintf(out, "P2\n%d %d\n%d\n", image->width, image->height, MAX_VALUE);

    for (lin = 0; lin < image->height; lin++)
    {

      for (col = 0; col < image->width; col++)
      {

        fprintf(out, col == 0 ? "%d" : " %d", image->matrix[lin][col]);
      }

      fprintf(out, "\n");
    }
  }

  fclose(out);
}

// funcao auxiliar: cria um ponto a partir de coordenadas x e y do tipo double.

Point create_point(double x, double y)
{

  Point p;

  p.x = (int)round(x);
  p.y = (int)round(y);

  return p;
}

// funcao que pinta um pixel da imagem com a cor especificada.

void set_pixel(Image *image, Point p, int color)
{

  if (p.x < 0 || p.y < 0 || p.x >= image->width || p.y >= image->height)
    return;

  image->matrix[p.y][p.x] = color;
}

// funcao que devolve o valor de um pixel da imagem.

int get_pixel(Image *image, Point p)
{

  if (p.x < 0 || p.y < 0 || p.x >= image->width || p.y >= image->height)
    return -1;

  return image->matrix[p.y][p.x];
}

// funcao que desenha uma linha conectando os pontos p1 e p2, com a cor especificada.

void draw_line(Image *image, Point p1, Point p2, int color)
{

  double x, y;
  int deltaX = abs(p1.x - p2.x);
  int deltaY = abs(p1.y - p2.y);

  Point a, b;

  if (deltaX >= deltaY)
  {

    a = p1.x < p2.x ? p1 : p2;
    b = p1.x < p2.x ? p2 : p1;

    for (x = a.x; x <= b.x; x++)
    {

      y = ((x - a.x) / deltaX) * (b.y - a.y) + a.y;
      set_pixel(image, create_point(x, y), color);
    }
  }
  else
  {

    a = p1.y < p2.y ? p1 : p2;
    b = p1.y < p2.y ? p2 : p1;

    for (y = a.y; y <= b.y; y++)
    {

      x = ((y - a.y) / deltaY) * (b.x - a.x) + a.x;
      set_pixel(image, create_point(x, y), color);
    }
  }
}

Point point_sum(Point p1, Point p2)
{
  Point p;
  p.x = p1.x + p2.x;
  p.y = p1.y + p2.y;
  return p;
}

Point point_sub(Point p1, Point p2)
{
  Point p;
  p.x = p1.x - p2.x;
  p.y = p1.y - p2.y;
  return p;
}

Point point_scalar_product(double k, Point p1)
{
  return create_point(k * p1.x, k * p1.y);
}

Point point_lt(double M[2][2], Point p1)
{
  return create_point(M[0][0] * p1.x + M[0][1] * p1.y,
                      M[1][0] * p1.x + M[1][1] * p1.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                            ////
////  as duas funcoes abaixo devem ser completadas para implementar as funcionalidades pedidas. ////
////                                                                                            ////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

// funcao que desenha a Curva de koch conectando os pontos p1 e p2, na cor especificada. O parametro
// 'limit' determina o tamanho minimo que um segmento da curva deve ter para que seja subdividido de
//  recursivamente em segmentos menores.

void koch_curve(Image *image, Point p1, Point p2, int limit, int color)
{
  int lineLength = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
  if (lineLength < limit)
  {
    draw_line(image, p1, p2, color);
    return;
  }
  Point A = point_sum(point_scalar_product(2.0 / 3.0, p1), point_scalar_product(1.0 / 3.0, p2));
  Point C = point_sum(point_scalar_product(1.0 / 3.0, p1), point_scalar_product(2.0 / 3.0, p2));

  // Cria o vetor entre p1 e p2
  Point V = point_sub(p2, p1);
  double k = 1.0 / sqrt(12.0);
  // Matriz de transformacao linear, rotacionando o vetor em 30 graus e diminuindo o valor dele
  double M[2][2] = {{0.5, k}, {-k, 0.5}};
  // Aplicando a transformacao linear
  Point V1 = point_lt(M, V);
  // somando o vetor ao ponto p1 para ter o ponto B
  Point B = point_sum(p1, V1);

  koch_curve(image, p1, A, limit, color);
  koch_curve(image, A, B, limit, color);
  koch_curve(image, B, C, limit, color);
  koch_curve(image, C, p2, limit, color);
}

// funcao que preenche uma região da imagem com a cor 'new_color' a partir do ponto 'p'. O preenchimento
// deve alterar a cor de todos os pixels presentas na regiao continua em torno de 'p' composta pelos pixels
// que possuem valor igual ao parametro 'original_color'. Na primeira chamada da funcao, o valor deste
// parâmetro deve corresponder ao valor de cor original pixel aonde o processo de preenchimento se inicia.

void region_fill(Image *image, Point p, int original_color, int new_color)
{
  if (get_pixel(image, p) == new_color)
  {
    return;
  }
  if (get_pixel(image, p) != original_color)
  {
    return;
  }
  set_pixel(image, p, new_color);

  region_fill(image, point_sum(p, create_point(1, 0)), original_color, new_color);
  region_fill(image, point_sum(p, create_point(0, 1)), original_color, new_color);
  region_fill(image, point_sum(p, create_point(0, -1)), original_color, new_color);
  region_fill(image, point_sum(p, create_point(-1, 0)), original_color, new_color);
}

// funcao main que implementa o programa principal. Precisa ser completada para
// que os novos comandos implementados sejam reconhecidos e chamados adequadamente.

int main(int argc, char **argv)
{

  char command[32];
  char *input_file_name, *output_file_name;
  int width, height, background_color, color;

  FILE *in;
  Image *img;
  Point p1, p2;

  if (argc != 3)
  {

    printf("uso: %s <entrada> <saida>\n", argv[0]);
    return 1;
  }

  input_file_name = argv[1];
  output_file_name = argv[2];
  in = fopen(input_file_name, "r");
  fscanf(in, "%d %d %d", &width, &height, &background_color);
  img = create_imagem(width, height, background_color);

  while (fscanf(in, "%s", command) == 1 && strcmp(command, END) != 0)
  {

    if (strcmp(command, SET_COLOR) == 0)
    {

      fscanf(in, "%d", &color);
    }
    else if (strcmp(command, SET_PIXEL) == 0)
    {

      fscanf(in, "%d %d", &p1.x, &p1.y);
      set_pixel(img, p1, color);
    }
    else if (strcmp(command, DRAW_LINE) == 0)
    {

      fscanf(in, "%d %d %d %d", &p1.x, &p1.y, &p2.x, &p2.y);
      draw_line(img, p1, p2, color);
    }
    else if (strcmp(command, KOCH_CURVE) == 0)
    {
      int limiar;
      fscanf(in, "%d %d %d %d %d", &p1.x, &p1.y, &p2.x, &p2.y, &limiar);
      koch_curve(img, p1, p2, limiar, color);
    }
    else if (strcmp(command, REGION_FILL) == 0)
    {
      fscanf(in, "%d %d", &p1.x, &p1.y);
      region_fill(img, p1, background_color, color);
    }
    else
    {

      printf("comando desconhecido: %s\n", command);
    }
  }

  save_image(img, output_file_name);
  free_image(img);

  return 0;
}

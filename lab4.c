// Rafael Paladini Meirelles  2111538
// Ricardo Bastos Leta Vieira 2110526

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define letras "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define numeros "0123456789"

char gera_letra()
{
  int i = rand() % 26;
  return letras[i];
}

char gera_numero()
{
  int i = rand() % 9;
  return numeros[i];
}

void gera_placa(char placa[9])
{
  int i = 0;
  int j = 0;
  
  for(int i=0; i<3; i++)
  {
    placa[i] = gera_letra();
  }

  for(j = 3; j < 7; j++)
  {
    placa[j] = gera_numero();
  }
  
  placa[7] = '\0';
  placa[8] = '\n';
}

void gera_placas(int n, char placas[n][9])
{
  for(int i=0; i < n; i++)
  {
    gera_placa(placas[i]);
  }
}

void escreve_arquivo(int n, FILE * arq)
{
  srand(time(NULL));
  char placas[n][9];
  gera_placas(n, placas);
  fwrite(placas, 9, n, arq);
}

int hash(int key, int k)
{
  int p1 = key & 0x000003FF;
  int p2 = key & 0x000FFC00;
  int i = (p1 ^ p2 + 15 *(int)pow(k, 2)) % 1031;
  
  return i;
}

int insere_placa(int * vetor, int key, int k)
{
  int pos = hash(key, k);
  
  if(vetor[pos] == -1)
  {
    vetor[pos] = key;
    
    return k;
  }
  else
  {
    return insere_placa(vetor, key, k+1);
  }
}

int placa_para_int(char * placa)
{
  int n = 0;
  int i = 0;
  int j = 0;
  
  for(i=0; i < 3; i++)
  {
    n += (placa[i] - 'A') * pow(26, i);
  }
  
  for(j=3; i < 7; i++)
  {
    n += (placa[j] - '0') * pow(9, (j - 3));
  }
  
  return n;
}

// preenche tabela hash com -1 em todos os elementos
void prepara_vetor(int vetor[1031])
{
  for(int i=0; i<1031; i++)
  {
    vetor[i] = -1;
  }
}

void insere(FILE * placas, int size, int * vetor)
{
  prepara_vetor(vetor);
  char vetor_de_placas[size][9];
  int i = 0;

  int col; // armazena o numero de colisoes em cada insercao
  int total_col = 0; // armazena o numero total de colisoes
  
  clock_t start_geral;
  clock_t end_geral;

  int s = fread(vetor_de_placas, 9, size, placas);
  
  if (s != size)
  {
    fclose(placas);
    fputs("Falha na leitura do arquivo",stderr);
    exit(1);
  }
 
  start_geral = clock();
  
  for(i = 0; i<size; i++)
  {
    col = insere_placa(vetor,  placa_para_int(vetor_de_placas[i]), 0);
    total_col += col;
  }
  
  end_geral = clock();
  
  printf("Insercoes finalizadas. Concluido em %ldms com %d colisoes\n", (10000 * (end_geral - start_geral)) / CLOCKS_PER_SEC, total_col);
}

// recebe um ponteiro para a tabela hash, uma string que é a placa e o número de colisões 
int busca(int * vetor, char *placa, int k)
{
  int key = placa_para_int(placa);
  int pos = hash(key, k);
  
  if(pos == -1)
  {
    fputs("Placa nao encontrada\n",stderr);
    return -1;
  }

  if(vetor[pos] == key)
  {
    return pos;
  }
  else
  {
    return busca(vetor, placa, k+1);
  }
}

// recebe um ponteiro para a tabela hash, uma string que é a placa e o número de colisões
void remove_da_hash(int *vetor, char *key, int k)
{
  int i = busca(vetor, key, k);

  if (i == -1)
  {
    return;
  }

  vetor[i] = -1;
}

// recebe o ponteiro para um arquivo com as placas que devem ser buscadas, o número de placas a ser buscadas e um ponteiro para a tabela hash onde as placas serão buscadas
void realiza_testes_busca(FILE *placas, int n_testes, int *tab_hash)
{
  char vetor_de_placas[n_testes][9];
  int i = 0;
  
  clock_t start_geral;
  clock_t end_geral;
  
  int s = fread(vetor_de_placas, 9, n_testes, placas);
    
  if (s != n_testes)
  {
    fclose(placas);
    fputs("Falha na leitura do arquivo\n",stderr);
    exit(1);
  }

  start_geral = clock();
  
  for(int i=0; i<n_testes; i++)
  {
    busca(tab_hash, vetor_de_placas[i], 0);
  }
  
  end_geral = clock();
  
  printf("%d buscas finalizadas. Concluido em %ld * 10^(-5) segundos.\n", n_testes, (100000 * (end_geral - start_geral)) / CLOCKS_PER_SEC);
}

// recebe o ponteiro para um arquivo com as placas que devem ser excluidas, o número de placas a ser excluidas e um ponteiro para a tabela hash de onde as placas serão retiradas
void realiza_testes_exclusao(FILE *placas, int n_testes, int *tab_hash)
{
  char vetor_de_placas[n_testes][9];
  int i = 0;
  
  clock_t start_geral;
  clock_t end_geral;
  
  int s = fread(vetor_de_placas, 9, n_testes, placas);
    
  if (s != n_testes)
  {
    fclose(placas);
    fputs("Falha na leitura do arquivo\n",stderr);
    exit(1);
  }

  start_geral = clock();
  
  for(int i=0; i<n_testes; i++)
  {
    remove_da_hash(tab_hash, vetor_de_placas[i], 0);
  }
  
  end_geral = clock();
  
  printf("%d exclusoes finalizadas. Concluido em %ld * 10^(-5) segundos.\n", n_testes, (100000 * (end_geral - start_geral)) / CLOCKS_PER_SEC);
}

int main()
{
  int size = 128;
  
  FILE * f = fopen("placas.txt","r");
  int * vetor = malloc(1031 * sizeof(int));
  insere(f, size, vetor);
  fclose(f);
  
  FILE * f1 = fopen("placas.txt", "r");
  realiza_testes_busca(f,size,vetor);
  fclose(f1);
  
  FILE * f2 = fopen("placas.txt", "r");
  realiza_testes_exclusao(f,size,vetor);
  fclose(f2);
  
  return 0;
}
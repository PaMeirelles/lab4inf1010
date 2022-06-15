#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define letras "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define numeros "0123456789"

char ** gera_placas(int n);

char gera_letra()
{
  int i = rand() % 26;
  return letras[i];
}

char gera_numero()
{
  int i = rand() % 10;
  return numeros[i];
}

char * gera_placa()
{
  char * placa = malloc(8);
  
  for(int i=0; i<3; i++)
  {
    placa[i] = gera_letra();
  }

  for(int i=3; i < 7; i++)
  {
    placa[i] = gera_numero();
  }
  
  placa[7] = '\0';
  
  return placa;
}

int placa_para_int(char * placa)
{
  int n = 0;
  
  for(int i=0; i < 3; i++)
  {
    n += (placa[i] - 'A') * pow(26, i);
  }
  
  for(int i=3; i < 7; i++)
  {
    n += (placa[i] - '0') * pow(10, (i - 3));
  }
  
  return n;
}

int insere_hash(int * vetor, int key, int (* f)(int, int, int *, int), int k, int magic_number)
{
  int i = f(key, k, vetor, magic_number);
  
  if(i != -1)
  {
    vetor[i] = key;
    
    return k;
  }
  else
  {
    return insere_hash(vetor, key, f, k+1, magic_number);
  }
}

int simple_division(int key, int k, int * vetor, int magic_number)
{
  int i = ((magic_number * key + (int)pow(k,2)) % 1033 );
  
  if(vetor[i] != -1)
  {
    return -1;
  }
  
  return i;
}

int dobra(int key, int k, int * vetor, int magic_number)
{
  int p1 = key & 0x000003FF;
  int p2 = key & 0x000FFC00;
  int i = (p1 ^ p2 + magic_number *(int)pow(k, 2)) % 1033;

  if(vetor[i] != -1)
  {
    return -1;
  }

  return i;
}

// usa o hash de dobra
int busca(int * vetor, int key, int k, int magic_number)
{
  int p1 = key & 0x000003FF;
  int p2 = key & 0x000FFC00;
  int i = (p1 ^ p2 + magic_number *(int)pow(k, 2)) % 1033;
  
  if(i == -1)
  {
    return -1;
  }

  if(vetor[i] == key)
  {
    return i;
  }
  else
  {
    return busca(vetor, key, k+1, magic_number);
  }
}

void remove_da_hash(int *vetor, int key, int k, int magic_number)
{
  int i = busca(vetor, key, k, magic_number);

  if(k == 0)
  {
    vetor[i] = -1;
  }
}

void testa_func(int(*f)(int, int, int*, int), int * vetor, int magic_number, int * colisoes, int * keys, int size)
{
  int collisions = 0;
  clock_t start;
  clock_t stop;
  
  start = clock();
  
  for(int i=0; i < size; i++)
  {
   collisions += insere_hash(vetor, keys[i], f, 0,magic_number);
  }
  
  stop = clock();
  
  *colisoes = collisions;
}

void bateria_de_testes(long tempo_limite, int(*f)(int, int, int*, int), int * vetor, int num_testes, int size_vetor)
{
  char ** placas;
  int * keys = malloc(size_vetor * sizeof(int));

  int magic_number = 6;
  int * melhores_colisoes = malloc(4 * num_testes);

  for(int i=0; i<num_testes; i++)
  {
    melhores_colisoes[i] = 0;
  }
  
  int coli;
  clock_t start = clock();
  
  while((clock() - start) < tempo_limite)
  {
    for(int t=0; t<num_testes; t++)
    {  
      placas = gera_placas(size_vetor);
      
      for(int j=0; j<size_vetor; j++)
      {
        keys[j] = placa_para_int(placas[j]);
      }
    
      testa_func(f, vetor, magic_number, &coli, keys, size_vetor);
    
      melhores_colisoes[magic_number - 1] += coli;
    
      for(int k=0; k<1033; k++)
      {
        vetor[k] = -1;
      }
  }
    printf("Finalizado teste para %d. Colisões restantes: %d. Tempo decorrido até o momento: %lds\n", magic_number, melhores_colisoes[magic_number-1]/num_testes, (clock()-start) / CLOCKS_PER_SEC);
    magic_number++;
  }
  
  for(int n=0; n<magic_number; n++)
  {
    printf("Número mágico %d produziu em média %d colisões\n", n+1, melhores_colisoes[n]/num_testes);
  }
}

char ** gera_placas(int n)
{
  char ** placas = malloc(8 * n);
  for(int i=0; i < n; i++)
  {
    placas[i] = gera_placa();
  }
  
  return placas;
}

int not_main(void) 
{
  srand(time(NULL));
  
  int * vetor = malloc(4 * 1033);
  
  for(int i=0; i<1033; i++)
  {
    vetor[i] = -1;
  }
  
  bateria_de_testes(100000000, dobra, vetor, 10000, 128);
  // bateria_de_testes(100000000, dobra, vetor, 10000, 256);
  // bateria_de_testes(100000000, dobra, vetor, 10000, 512);
  
  return 0;
}
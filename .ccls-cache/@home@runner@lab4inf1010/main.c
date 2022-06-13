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
  int i = ((magic_number * key + k) % 1033 );
  
  if(vetor[i] != -1)
  {
    return -1;
  }
  
  return i;
}

void testa_func(int(*f)(int, int, int*, int), int * vetor, int magic_number, int * colisoes, int * keys)
{
  int collisions = 0;
  clock_t start;
  clock_t stop;
  
  start = clock();
  for(int i=0; i < 512; i++)
  {
   collisions += insere_hash(vetor, keys[i], simple_division, 0,magic_number);
  }
  
  stop = clock();
  
  *colisoes = collisions;
  }
void bateria_de_testes(long tempo_limite, int(*f)(int, int, int*, int), int * vetor, int num_testes)
{
  char ** placas;
  int * keys = malloc(512 * sizeof(int));

  int magic_number = 1;
  int * melhores_colisoes = malloc(4 * num_testes);

  for(int i=0; i<num_testes; i++)
  {
    melhores_colisoes[i] = 0;
  }
  
  int coli;
  clock_t start = clock();
  while((clock() - start) < tempo_limite){
    for(int t=0; t<num_testes; t++){  
      placas = gera_placas(512);
      for(int i=0; i<512; i++){
        keys[i] = placa_para_int(placas[i]);
      }
    testa_func(f, vetor, magic_number, &coli, keys);
    melhores_colisoes[magic_number - 1] += coli;
    for(int i=0; i<1033; i++)
    {
      vetor[i] = -1;
    }
    }
    printf("Finalizado teste para %d. Colisões restantes: %d. Tempo decorrido até o momento: %lds\n", magic_number, melhores_colisoes[magic_number-1]/num_testes, (clock()-start) / CLOCKS_PER_SEC);
    magic_number++;
  }
  for(int i=0; i<magic_number; i++){
    printf("Número mágico %d produziu em média %d colisões\n", i+1, melhores_colisoes[i]/num_testes);
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

int main(void) 
{
  srand(time(NULL));
  
  int * vetor = malloc(4 * 1033);
  
  for(int i=0; i<1033; i++)
  {
    vetor[i] = -1;
  }
  
  bateria_de_testes(100000000, simple_division, vetor, 1000);
  return 0;
}
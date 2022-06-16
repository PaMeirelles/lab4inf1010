#include "preparacao.h"

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
  for(int i=0; i<3; i++)
  {
    placa[i] = gera_letra();
  }

  for(int i=3; i < 7; i++)
  {
    placa[i] = gera_numero();
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
#include "preparacao.h"
#include "insere.h"
#define SIZE 256
int main(){
  FILE * f = fopen("placas.txt", "w");
  escreve_arquivo(SIZE, f);
  fclose(f);
  
  f = fopen("placas.txt","r");
  insere(f, SIZE);
  fclose(f);
  return 0;
}
#include "preparacao.h"

int main(){
  FILE * f = fopen("placas.txt", "w");
  escreve_arquivo(128, f);
  fclose(f);
  return 0;
}
#include "preparacao.h"
#include "insere.h"
#include "testes.h"
#define SIZE 512
/*
Disclaimer: O enunciado pede para utilizar o vetor com tamanho X, sendo X o maior número primo após de 1024. Visto que, após perguntar ao professor, ele nos informou que a interpretação era parte da avaliação, tomamos a liberdade de considerar X o maior número primo abaixo de 1024. Fizemos isso pois encontrar o maior número primo acima de 1024 seria impossível, visto que já foi provado que existem infinitos números primos. Segue a prova abaixo
https://primes.utm.edu/notes/proofs/infinite/euclids.html
  */
int main(){
  
  FILE * f = fopen("placas.txt", "w");
  escreve_arquivo(SIZE, f);
  fclose(f);
  
  f = fopen("placas.txt","r");
  int * vetor = malloc(1031 * 4);
  insere(f, SIZE, vetor);
  fclose(f);
  
  //realiza_testes(512, 10000000);
  return 0;
}
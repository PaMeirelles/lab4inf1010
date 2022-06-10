#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define letras "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define numeros "0123456789"

char gera_letra(){
  int i = rand() % 26;
  return letras[i];
}

char gera_numero(){
  int i = rand() % 10;
  return numeros[i];
}

char * gera_placa(){
  char * placa = malloc(8);
  for(int i=0; i<3; i++){
    placa[i] = gera_letra();
  }
  for(int i=3; i < 7; i++){
    placa[i] = gera_numero();
  }
  placa[7] = '\0';
  return placa;
}

char ** gera_placas(int n){
  char ** placas = malloc(8 * n);
  for(int i=0; i < n; i++){
    placas[i] = gera_placa();
    }
  return placas;
}
int main(void) {
  
  srand(time(NULL));
  char ** placas = gera_placas(20);
  while(*placas){
    
  printf("%s\n", *placas);
    placas++;
  }
  return 0;
}
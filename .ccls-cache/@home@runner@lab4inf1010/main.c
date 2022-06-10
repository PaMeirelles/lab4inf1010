#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

int placa_para_int(char * placa){
  int n = 0;
  for(int i=0; i < 3; i++){
    n += (placa[i] - 'A') * pow(26, i);
  }
  for(int i=3; i < 7; i++){
    n += (placa[i] - '0') * pow(10, (i - 3));
  }
  return n;
}

int insere_hash(char ** vetor, char * key, int (* f)(char *, int, char **), int k){
  int i = f(key, k, vetor);
  if(i != -1){
    vetor[i] = key;
    return k;
  }
  else{
    return insere_hash(vetor, key, f, k+1);
  }
}

int simple_division(char * key, int k, char ** vetor){
  int i = 0;
  return 0;
}
char ** gera_placas(int n){
  char ** placas = malloc(8 * n);
  for(int i=0; i < n; i++){
    placas[i] = gera_placa();
    }
  return placas;
}
int main(void) {
  return 0;
}
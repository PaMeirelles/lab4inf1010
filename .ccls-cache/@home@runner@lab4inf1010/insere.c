#include "insere.h"

int hash(int key, int k)
{
  int p1 = key & 0x000003FF;
  int p2 = key & 0x000FFC00;
  int i = (p1 ^ p2 + 15 *(int)pow(k, 2)) % 1031;
  
  return i;
}

int insere_placa(int * vetor, int key, int k)
{
  int i = hash(key, k);
  
  if(vetor[i] == -1)
  {
    vetor[i] = key;
    
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
  
  for(int i=0; i < 3; i++)
  {
    n += (placa[i] - 'A') * pow(26, i);
  }
  
  for(int i=3; i < 7; i++)
  {
    n += (placa[i] - '0') * pow(9, (i - 3));
  }
  
  return n;
}
void prepara_vetor(int vetor[1031]){
  for(int i=0; i<1031; i++){
    vetor[i] = -1;
  }
}
void insere(FILE * placas, int size, int * vetor)
{
  prepara_vetor(vetor);
  char vetor_de_placas[size][9];
  
  int col;
  int total_col = 0;
  
  clock_t start_geral;
  clock_t end_geral;
  
  int s = fread(vetor_de_placas, 9, size, placas);
  if (s != size){
    fclose(placas);
    fputs("Falha na leitura do arquivo",stderr);
    exit(1);
  }
  start_geral = clock();
  for(int i=0; i<size; i++){
    col = insere_placa(vetor,  placa_para_int(vetor_de_placas[i]), 0);
    total_col += col;
    printf("Inserido a placa %s. Colisoes: %d\n", vetor_de_placas[i], col);
  }
  end_geral = clock();
  printf("Insercoes finalizadas. Concluido em %ldms com %d colisoes\n", (10000 * (end_geral - start_geral)) / CLOCKS_PER_SEC, total_col);
}
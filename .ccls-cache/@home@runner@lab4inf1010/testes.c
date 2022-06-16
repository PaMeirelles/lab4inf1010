#include "testes.h"
#include "insere.h"
#include "preparacao.h"

void insere_dic(int testes[100][2], int num_col, int * ultimo_el){
  for(int i=0; i<=*ultimo_el; i++){
    if(testes[i][0] == num_col){
      testes[i][1] += 1;
      return;
    }
  }
  testes[*ultimo_el] [0] = num_col;
  testes[*ultimo_el] [1] = 1;
  (*ultimo_el)++;
}

int comp(const void *elem1, const void *elem2){
  int e1 = *(((int *)elem1));
  int e2 = *(((int *)elem2));
  if(e1 > e2) return 1;
  if(e2 > e1) return -1;
  return 0;
}

void realiza_testes(int n_chaves, int n_testes){
  int testes[1000][2];
  int vetor[1031];
  int col;
  int total_col;
  char placa[9];
  int ultimo_el = 0;

  for(int t=0; t<n_testes; t++){
    prepara_vetor(vetor);
    total_col = 0;   
    for(int c=0; c<n_chaves; c++){
      gera_placa(placa);  
      col = insere_placa(vetor, placa_para_int(placa), 0);
      total_col += col;
      
    }   
    insere_dic(testes, total_col, &ultimo_el);
    if((t+1) % (n_testes / 100) == 0){
      printf("%d%%\n", 100 * (t+1) / n_testes);
    }
  }
  qsort(testes, ultimo_el, 8, comp);
  for(int i=0; i<ultimo_el; i++){
    printf("%d,%d\n", testes[i][0], testes[i][1]);
  }
}

int busca(int * vetor, char *placa, int k)
{
  int key = placa_para_int(placa);
  int pos = hash(key, k);
  
  if(pos == -1)
  {
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

void remove_da_hash(int *vetor, char *key, int k)
{
  int i = busca(vetor, key, k);

  vetor[i] = -1;
}


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
    fputs("Falha na leitura do arquivo",stderr);
    exit(1);
  }

  start_geral = clock();
  
  for(int i=0; i<n_testes; i++)
  {
    busca(tab_hash, vetor_de_placas[i], 0);
  }
  
  end_geral = clock();
  
  printf("%d buscas finalizadas. Concluido em %ld * 10^(-7) segundos.\n", n_testes, (10000000 * (end_geral - start_geral)) / CLOCKS_PER_SEC);
}

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
    fputs("Falha na leitura do arquivo",stderr);
    exit(1);
  }

  start_geral = clock();
  
  for(int i=0; i<n_testes; i++)
  {
    remove_da_hash(tab_hash, vetor_de_placas[i], 0);
  }
  
  end_geral = clock();
  
  printf("%d exclusoes finalizadas. Concluido em %ld * 10^(-7) segundos.\n", n_testes, (10000000 * (end_geral - start_geral)) / CLOCKS_PER_SEC);
}
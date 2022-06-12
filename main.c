#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define letras "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define numeros "0123456789"

void print_array(int *vetor, int size)
{
    for(int i=0; i < size; i++)
    {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

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

int simple_division(int key, int k, int * vetor, int magic_number)
{
  int i = ((magic_number * key + k) % 1033);

// retirei pq tava atrapalhando a remocao, o teste de ver se esta vazio ou nao fica na insere_hash agr
//   if(vetor[i] != -1)
//   {
//     return -1;
//   }
  
  return i;
}

int insere_hash(int * vetor, int key, int (* f)(int, int, int *, int), int k, int magic_number, int *tem_colisao)
{
  int i = f(key, k, vetor, magic_number);
  
  if(vetor[i] == -1)  /*if(i != -1), veja funcao simple_division para ver pq mudou*/
  {
    vetor[i] = key;
    
    return k;
  }
  else
  {
    *tem_colisao = 1;
    return insere_hash(vetor, key, f, k+1, magic_number, tem_colisao);
  }
}

void remove_hash(int * vetor, int key, int (* f)(int, int, int *, int), int k, int magic_number)
{
    // encontra o indice do elemento
    int pos = search_hash(vetor, key, f, k, magic_number);
    
    // se nao achou, mostra mensagem de erro
    if(pos == -1)
    {
        perror("Erro ao remover, elemento nao encontrado");
    }
    
    // se achou, muda o valor na posicao dada para -1
    vetor[pos] = -1;
}

/*
    Funcao que retorna a posicao de um elemento na tabela hash, se ele existir
    Retorna -1 se o elemento nao existir

    Calcular o hash do elemento
    Verificar se o elemento existe na posicao correspondente ao hash gerado
        Se existir, retorna a posicao do elemento
        Caso contrario, se o elemento na posicao da hash gerada for 0
            Iterar pelo resto da hash ate encontrar um elemento igual a -1 ao igual a key
        
        Se percorrer a hash inteira sem encontra o valor, retorna -1
*/
int search_hash(int *vetor, int key, int (* f)(int, int, int *, int), int k, int magic_number)
{
    int i = f(key, k, vetor, magic_number);
    
    if(vetor[i] == key)
    {
        return i;
    }
    
    if(vetor[i] == 0)
    {
        for(; i < 1033; i++)
        {
            if(vetor[i] == key)
            {
                return i;
            }
        }
    }

    return -1;
}

void testa_insert(int(*f)(int, int, int*, int), int * vetor, int magic_number, int * colisoes, long * tempo, int * keys, int size)
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
  *tempo = stop-start;
}

void testa_remocao(int(*f)(int, int, int*, int), int * vetor, int magic_number, long * tempo, int * keys, int size)
{
  clock_t start;
  clock_t stop;
  
  start = clock();
  for(int i=0; i < (size/2); i++)
  {
    remove_hash(vetor, keys[i], f, 0, magic_number);
  }
  
  stop = clock();
  
  *tempo = stop-start;
}

void bateria_de_testes_insert(long tempo_limite, int(*f)(int, int, int*, int), int * vetor, int size)
{
  char ** placas;
  int * keys = malloc(size * sizeof(int));

  int magic_number = 1;
  int melhor_colisao = 1000000;

  int coli;
  long tempo;  
  placas = gera_placas(size);
  
  for(int i=0; i<size; i++)
  {
    keys[i] = placa_para_int(placas[i]);
  }
  
  clock_t start = clock();
  while(clock() - start < tempo_limite)
  {  
    testa_insert(f, vetor, magic_number, &coli, &tempo, keys, size);
    
    if(coli < melhor_colisao)
    {
      printf("Novo recorde! %d produziu %d colisões (%d a menos que o anterior) tempo: %ldms\n", magic_number, coli, melhor_colisao-coli, tempo);
      melhor_colisao = coli;
    }
    
    for(int i=0; i<1024; i++)
    {
      vetor[i] = -1;
    }
    
    magic_number++;
  }
}

void bateria_de_testes_remocao(long limite, int(*f)(int, int, int*, int), int *vetor, int size)
{
    char ** placas;
    int * keys = malloc(size * sizeof(int));

    int magic_number = 1;

    int coli;
    long tempo;  
    placas = gera_placas(size);
    
    for(int i=0; i < size; i++)
    {
        keys[i] = placa_para_int(placas[i]);
    }

    for(int j = 0; j < size; j++)
    {
        insere_hash(vetor, keys[j], simple_division, 0, magic_number);
    }

    // Usei para debugar a remocao, o loop embaixo printa a hash e copia ela para um vetor auxiliar
    // o vetor auxiliar eh usado para comparar o antigo com o novo hash, apos a remocao de metade
    // das chaves, se o novo hash eh igual ao antigo, entao a remocao nao funcionou e 
    // se o numero de posicoes diferentes for diferente de size/2, entao a remocao nao funcionou
   
    // printf("Inserindo %d placas\n", size);
    // print_array(vetor, size);
    int * vetor_aux = malloc(size * sizeof(int));
    for (int n = 0; n < size; n++)
    {
        vetor_aux[n] = vetor[n];
    }

    // testa remocao sem cronometrar e sem alterar o magic_number
    // for(int i = 0; i < size/2; i++)
    // {
    //     remove_hash(vetor, keys[i], simple_division, 0, magic_number);
    // }
  
    /*
    printf("\nApos remocao\n");
    print_array(vetor, size);

    printf("\n");
    int sao_iguais = 0;
    for (int n = 0; n < size; n++)
    {
        if(vetor_aux[n] != vetor[n])
        {
            sao_iguais = 1;
            printf("Diferentes na pos %d;original = %d, novo = %d\n", n, vetor_aux[n], vetor[n]);
        }
    }
 
    printf("sao_iguais = %d\n", sao_iguais);
    */
    for (magic_number = 1;magic_number < limite; magic_number++)
    {  
        printf("Teste\n");
        // for(int i=0; i < size; i++)
        // {
        // printf("Teste\n");
        for(int j=0; j < size; j++)
        {
            vetor[j] = vetor_aux[j];
        }

        // printf("Teste\n");
        testa_remocao(f, vetor, magic_number, &tempo, keys, size);
        printf("Para um vetor com %d placas, a remocao de metade delas levou %d usando magic number %d\n", size, tempo, magic_number);
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
  
  int * vetor = malloc(1033 * sizeof(int));
  int size = 512;

  for(int i=0; i<1024; i++)
  {
    vetor[i] = -1;
  }
  
//   bateria_de_testes_insert(1000, simple_division, vetor, size);

  bateria_de_testes_remocao(1000, simple_division, vetor, size);

    return 0;
}

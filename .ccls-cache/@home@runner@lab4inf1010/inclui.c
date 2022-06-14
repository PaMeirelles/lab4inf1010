#include <math.h>

int hash(int key, int k)
{
  int p1 = key & 0x000003FF;
  int p2 = key & 0x000FFC00;
  int i = (p1 ^ p2 + 15 *(int)pow(k, 2)) % 1033;
  
  return i;
}

int insere(int * vetor, int key, int k)
{
  int i = hash(key, k);
  
  if(vetor[i] == -1)
  {
    vetor[i] = key;
    
    return k;
  }
  else
  {
    return insere(vetor, key, k+1);
  }
}

#include <stdio.h>
#include <stdlib.h>

typedef struct array2D *matrix;
typedef struct chain_node chain;
typedef struct hashbrowns *hash;

struct array2D
{
  int **mat;
  int sizeX;
  int sizeY;
  int nimber;
};

struct chain_node
{
  matrix data;
  struct chain_node *next;
  struct chain_node *moveSplit;
};

struct hashbrowns
{
  chain **table;
  int capacity;
  int size;
};


//####################################################//
//                 Implementations                    //
//####################################################//

//##############################//
//           Matrix             //
//##############################//

matrix newMatrix(int sizeX, int sizeY, int nimber)
{
  if (sizeX <= 0 || sizeY <= 0) { return NULL; }

  int i = 0;
  matrix board = (matrix) malloc(sizeof(struct array2D));
  board->mat = (int**)malloc(sizeY * sizeof(int*));
  board->sizeX = sizeX;
  board->sizeY = sizeY;
  board->nimber = nimber;

  while (i < sizeY)
  {
    int j = 0;
    int *arr = (int*)malloc(sizeX * sizeof(int));
    board->mat[i] = arr;

    while (j < sizeX)
    {
      board->mat[i][j] = 1;
      j++;
    }
    i++;
  }

  return board;
}

void printMatrix(matrix M)
{
  if (M == NULL) { return; }
  printf("Board Position: \n");
  int i = 0;
  while (i < M->sizeY)
  {
    int j = 0;
    while(j < M->sizeX)
    {
   //   printf("i: %d, j: %d, mY: %d, mX: %d\n", i,j,M->sizeY,M->sizeX);
      printf("%d \0", M->mat[i][j]);
      j++;
    }
    i++;
    printf("\n");
  }
}





//#########################//
//         Chain           //
//#########################//

chain *newChain(matrix M)
{
  chain *C = (chain*)malloc(sizeof(struct chain_node));
  C->data = M;
  C->next = NULL;
  C->moveSplit = NULL;
  return C;
}


chain *addChain(chain *C, matrix M1, matrix M2)
{
  chain *p = newChain(M1);
  chain *q = newChain(M2);
  if (M2 != NULL)
  {
    chain *q = newChain(M2);
    p->moveSplit = q;
  }

  p->next = C;
  C = p;
  if (C->data == NULL) { printf("Failure\n"); }
  return C;
}

void freeChain(chain *C)
{
  while (C != NULL) {
    chain *P = C->moveSplit;
    while (P != NULL) {
     // freeMatrix(P->data);
      if (P->next != NULL) {
        freeChain(P->next);
      }
      chain *Q = P;
      P = P->moveSplit;
      free(Q);
    }
    free(P);
    free(C->data);
    chain *D = C;
    C = C->next;
    free(D);
  }
  free(C);
}

//#########################//
//       Hash Table        //
//#########################//
hash newHashtable(int capacity, int size)
{
  hash H = (hash)malloc(sizeof(struct hashbrowns));
  chain **table = (chain**)malloc(capacity * sizeof(chain));

  H->table = table;
  H->capacity = capacity;
  H->size = size;
  return H;
}

// Hash the element
int hashKey(matrix e)
{
  int k = 0;
  int i = 0;
  while (i < e->sizeY)
  {
    int j = 0;
    int colSum = 0;
    while (j < e->sizeX)
    {
      if (e->mat[i][j] == 1)
      {
        colSum = 2 * colSum + e->mat[i][j];
      }
      j++;
    }
    i++;
    k += colSum;
  }
  return k;

}

int matrixEqual(matrix e1, matrix e2, int k)
{
  if (e1 == NULL && e2 == NULL)
  {
    return 1;
  }
  else if (e1 == NULL || e2 == NULL)
  {
    return 0;
  }
  else if (hashKey(e1) == k)
  {
    int end = 0;
    int i = 0;
    int m = 0;
    int n = 0;
    while (i < e1->sizeY && m < e2->sizeY && end == 0)
    {
      int j = 0;
      int n = 0;
      while (j < e1->sizeX && n < e2->sizeX && end == 0)
      {
        if (e1->mat[i][j] == 1 && e2->mat[m][n] == 1)
        {
          end = 1;
          while (j < e1->sizeX && n < e2->sizeX)
          {
            if (e1->mat[i][j] != e2->mat[m][n]) { return 0; }
            j++;
            n++;
          }
        }
        else if (e1->mat[i][j] == 1)
        {
          n++;
        }
        else if (e2->mat[i][j] == 1)
        {
          j++;
        }
        else {
          j++;
          n++;
        }
      }
      i++;
      m++;
    }
    return 1;
  }
  return 0;
}

void hashInsert(hash H, matrix e)
{
  int k = hashKey(e);
  int i = abs(k % H->capacity);

  chain *p = H->table[i];
  while (p != NULL)
  {
    if (matrixEqual(p->data, e, k))
    {
      p->data = e;
      return;
    }
    p = p->next;
  }

  chain *q = newChain(e);
  q->next = H->table[i];
  q->moveSplit = NULL;
  H->table[i] = q;
  H->size++;
}




matrix hashLookup(hash H, matrix e)
{
  if (e == NULL) { return NULL; }
  int hashK = hashKey(e);
  int i = abs(hashK % H->capacity);

  chain *p = H->table[i];
  while (p != NULL)
  {
    if (p->data == NULL) { return NULL; }
    else if (matrixEqual(p->data, e, hashK))
    {
      return (matrix)p->data;
    }
    p = p->next;
  }

  return NULL;
}


void freeHash(hash H)
{
  int i = 0;
  while (i < H->capacity) {
    freeChain(H->table[i]);
    i++;
  }
  free(H->table);
  free(H);
}



void printHashtable(hash H)
{
  printf("The hashtable has a capacity of: %d\n\n", H->capacity);
  int i = 0;
  while (i < H->capacity)
  {
    printf("Table Index: %d\n", i);
    chain *C = H->table[i];
    int j = 0;
    while (C != NULL)
    {
      matrix M = C->data;
      if (M != NULL) {
        printMatrix(M);
        printf("The Nimber of this board is:   %d\n", M->nimber);

        chain *Q = C->moveSplit;
        while (Q != NULL && Q->data != NULL)
        {
          printMatrix(Q->data);
          Q = Q->moveSplit;
        }
      }
      C = C->next;
      j++;
    }
    i++;
  }
}






#include <stdio.h>
#include "pennyTable.h"

//##############################################//
//           Function Declarations              //
//##############################################//
int isValidMove(matrix, int, int, int, int, int);
int makeMove(matrix, int, int, int ,int, int);
chain *isolate(matrix, chain*, int, int, int, int);
void merge(int*, int, int, int);
void mergeSort(int*, int, int);
int calculateNimber(matrix, chain*, hash, int);
void evalMoves(matrix, hash);
void main();


void testIsolate();



//###############################################//
//              Implementations                  //
//###############################################//


// Determines if a given move is valid.
int isValidMove(matrix M, int moveX0, int moveY0,
                          int moveX1, int moveY1, int play)
{
  if (moveX1 >= M->sizeX || moveY1 >= M->sizeY) { return 0; }

  int i = moveY0;
  while (i <= moveY1) {
    int j = moveX0;
    while (j <= moveX1) {
      if (j >= M->sizeX || i >= M->sizeY) { return 0; }
      if (M->mat[i][j] == (!play)) { return 0; }
      j++;
    }
    i++;
  }
  return 1;
}



// Performs or undoes a move on the board based on play
int makeMove(matrix M, int moveX0, int moveY0,
                       int moveX1, int moveY1, int play)
{
  if ((play != 0 && play != 1) || moveX1 >= M->sizeX || moveY1 >= M->sizeY)
  {
    return 0;
  }
  int i = moveY0;
  while (i <= moveY1) {
    int j = moveX0;
    while (j <= moveX1) {
      M->mat[i][j] = play;
      j++;
    }
    i++;
  }
  return 1;
}

// Finds and isolate possible separated positions in the matrix.
chain *isolate(matrix M, chain *potPos,
            int moveX0, int moveY0, int moveX1, int moveY1)
{
  if (potPos == NULL) { potPos = newChain(NULL); }
  int broken = 0;

  // If the matrix is broken up by 2 vertical 0s
  int i = moveY0;
  while (i <= moveY1) {
    int j = moveX0;
    while (j <= moveX1) {
      if (i < 0 || i >= M->sizeY || j < 0 || j >= M->sizeX) { break; }
      if (M->mat[i][j] == 0 && M->mat[(i+1) % M->sizeY][j] == 0)
      {
        printf("Case 1, vertical break\n");
        broken = 1;

        // Create two matrices, one on each side of the break.
        matrix M1 = newMatrix(j+1, M->sizeY, -1);
        int a = 0;
        while (a < M->sizeY) {
          if (M1 == NULL) { a++; continue; }
          int b = 0;
          while (b < j+1) {
            M1->mat[a][b] = M->mat[a][b];
            b++;
          }
          a++;
        }

        matrix M2 = newMatrix(M->sizeX - j - 1, M->sizeY, -1);
        a = 0;
        while (a < M->sizeY) {
          if (M2 == NULL) { a++; continue; }
          int b = moveX1;
          while (b < M->sizeX) {
            M2->mat[a][b - moveX1] = M->mat[a][b];
            b++;
          }
          a++;
        }

        // Add the new split matrices to the chain
        potPos = addChain(potPos, M1, M2);
        break;
      }
      j++;
    }
    i++;
  }

  // If the matrix is broken up by top left and bottom right 0s
  if (broken == 0 &&
      (((M->mat[moveY0][moveX0] == 0) &&
        (M->mat[(moveY0 + 1) % M->sizeY][moveX1] == 0)) ||
       ((M->mat[moveY0][moveX1] == 0) &&
        (M->mat[(moveY0 + 1) % M->sizeY][moveX0] == 0))))
  {
    printf("Case 2, diagonal cut\n");
    printMatrix(M);
    broken = 1;

    // Create two matrices, one on each side of the break
    matrix M1 = newMatrix(moveX1, M->sizeY, -1);
    int a = 0;
    while (a < M->sizeY) {
      if (M1 == NULL) { a++; continue; }
      int b = 0;
      while (b < moveX1) {
        M1->mat[a][b] = M->mat[a][b];
        b++;
      }
      a++;
    }

    matrix M2 = newMatrix(M->sizeX - moveX1, M->sizeY, -1);

    a = 0;
    while (a < M->sizeY) {
      if (M2 == NULL) { a++; continue; }
      int b = moveX1;
      while (b < M->sizeX) {
        M2->mat[a][b - moveX1] = M->mat[a][b];
        b++;
      }
      a++;
    }

    // Add the new split matrices to the chain
    if (M1 == NULL) {
      potPos = addChain(potPos, M2, M1);
    }
    else
    {
      potPos = addChain(potPos, M1, M2);
    }
  }

  else if (M->sizeX == 1 & (M->mat[moveY0][0] == 0 && M->mat[moveY1][0] == 1))
  {
    printf("Test\n");
  }


  if (broken == 0)
  {
    printf("Case 3, broken = 0\n");
    printMatrix(M);
    potPos = addChain(potPos, M, NULL);
  }

  return potPos;
}



// Merge Sort to make finding the Mex of Nimbers easier
void merge(int *nimList, int lo, int mid, int hi)
{
  int *temp = (int*)malloc((hi - lo) * sizeof(int));
  int i = 0;
  int j = 0;
  int k = lo;
  while (k < hi) {
    if (i == mid - lo)
    {
      temp[i+j] = nimList[mid + j];
      j++;
    }
    if (j == hi - mid)
    {
      temp[i+j] = nimList[lo + i];
      i++;
    }

    else if (nimList[i+lo] <= nimList[j+mid])
    {
      temp[i+j] = nimList[lo + i];
      i++;
    }
    else
    {
      temp[i + j] = nimList[mid + j];
      j++;
    }

    k++;
  }

  k = lo;
  while (k < hi) {
    nimList[k] = temp[k - lo];
    k++;
  }
}

void mergeSort(int *nimList, int lo, int hi)
{
  if (hi-lo <= 1) return;
  int mid = lo + (hi-lo) / 2;

  mergeSort(nimList, lo, mid);
  mergeSort(nimList, mid, hi);
  merge(nimList, lo, mid, hi);
  return;
}


// Does the calculations to compute the Nimber of a given Game position
int calculateNimber(matrix M, chain *potPos, hash H, int sizeY)
{
  // Creating a list to hold nimber values and setting values.
  int *nimList = (int*)malloc(5 * sizeY * sizeof(int));
  chain *Q = potPos;
  int i = 0;
  while (i < 5 * sizeY * sizeof(int)) {
    nimList[i] = 5 * sizeY * sizeof(int);
    i++;
  }

  i = 0;
  int j = 0;
  while (Q != NULL && Q->data != NULL) {
    printf("NOTICE\n");
    printMatrix(Q->data);
    matrix N = hashLookup(H, Q->data);
    // If the matrix isn't in the hash table, or the nimber hasn't been calculated yet.
    if (N == NULL || N->nimber == -1)
    {
      // To prevent infinite recursion.
      if (matrixEqual(N, M, hashKey(M)) == 1)
      {
        Q = Q->next;
        continue;
      }
      else{
        evalMoves(Q->data, H);
      }
    }

    // Now that its nimber has been calculated, get the matrix.
    matrix P = hashLookup(H, Q->data);
    int curNim = P->nimber;

    // If there are multiple boards resulting from a move, get the xor of the nimbers
    chain *split = Q->moveSplit;
    while (split != NULL) {
      if (split->data == NULL)
      {
        break;
      }

      matrix K = hashLookup(H, split->data);
      if (K == NULL)
      {
        evalMoves(split->data, H);
      }
      matrix L = hashLookup(H, split->data);
      curNim = curNim ^ L->nimber;
      split = split->moveSplit;
    }
    if (i >= 5 * sizeY * sizeof(int))
    {
      continue;
    }

    nimList[i] = curNim;
    i++;
    Q = Q->next;
  }

  // If the board is the empty board, return 0.
  if (i == 0) { return 0; }


  // Sort the nimber list
  mergeSort(nimList, 0, i-1);
  i = 0;
  j = 0;
  int k = -1;

  // Compute the MEX of the nimbers
  while (j < 5 * sizeY * sizeof(int)) {
    printf("NimList nums: %d\n", nimList[j]);
    if (nimList[j] - 1 != k && nimList[j] != k)
    {
      return i;
    }
    else if (nimList[j] - 1 == k)
    {
      k++;
      i++;
    }
    j++;
  }
  free(nimList);
  return -1;
}


// Plays moves and provides calculateNimber with all necessary info
void evalMoves(matrix M, hash H)
{
  // Creating a chain of potential positions.
  chain *potPos = NULL;

  int i = 0;
  while (i < M->sizeY) {
    int j = 0;
    while (j < M->sizeX) {
      int a = 0;
      while (a <= 1) {
        int b = 0;
        while (b <= 1) {
          int moveX1 = j + b;
          int moveY1 = i + a;

          if (isValidMove(M, j, i, moveX1, moveY1, 1) == 1)
          {

            // Make a move, find the resulting games, and undo that move
            makeMove(M, j, i, moveX1, moveY1, 0);
            potPos = isolate(M, potPos, j, i, moveX1, moveY1);
            makeMove(M, j, i, moveX1, moveY1, 1);
          }
          b++;
        }
        a++;
      }
      j++;
    }
    i++;
  }
  hashInsert(H, M);

  if (potPos == NULL) {
    M->nimber = 0;
  }
  else {
    int nimber = calculateNimber(M, potPos, H, M->sizeY);
    M->nimber= nimber;
  }
  printf("What is this?\n");
  printMatrix(M);
  //freeChain(potPos);
  hashInsert(H, M);
}


void main()
{
  hash H = newHashtable(100, 100);
  //testIsolate();
  matrix zero = newMatrix(1,1,0);
  zero->mat[0][0] = 0;
  hashInsert(H,zero);

  matrix test1 = newMatrix(1,2,1);
  test1->mat[0][0] = 0;
  hashInsert(H,test1);

  matrix test2 = newMatrix(2,1,1);
  test1->mat[0][0] = 0;
  hashInsert(H,test2);

  int maxHeight = 5;
  int maxWidth = 2;

  // For all n by m matrices up to the max size, calculate nimbers.
  int m = 1;
  while (m <= maxHeight)
  {
    int n = 1;
    while (n <= maxWidth)
    {
      matrix M = newMatrix(m, n, -1);
      evalMoves(M, H);
      n++;
    }
    m++;
  }
  printHashtable(H);
  //freeHash(H);
}

















// Test function for Isolate. Seems to be working properly now...
void testIsolate()
{
  matrix M = newMatrix(2,2,-1);
  M->mat[0][1] = 0;
  M->mat[1][0] = 0;
  chain *P = newChain(NULL);
  P = isolate(M, P, 1,0,1,0);

  int i = 1;
  while (P != NULL)
  {
    printf("Size of P: %d\n", i);

    if (P->data != NULL)
    {
      printMatrix(P->data);
      if (P->moveSplit != NULL) { printMatrix(P->moveSplit->data); }
    }


    i++;
    P = P->next;
  }
  printf("Done Testing\n");
}

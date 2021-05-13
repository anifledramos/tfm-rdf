#ifndef ___MERGE
#define ___MERGE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


typedef struct
{ uint* sources;
  uint value;
} Result2;

void mergeMiguel(Result2 ** vectores, int numVectores);

#endif

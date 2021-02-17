#ifndef ___DACI
#define ___DACI

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#include "mymalloc.h"
#include "basic.h"
#include "bitrankw32int.h"

typedef struct sFTRepI {
	  uint listLength;
	  byte nLevels;
	  uint tamCode;
	  uint * levels;
	  uint * levelsIndex;
	  uint * iniLevel;
	  uint * rankLevels;
	  bitRankW32Int * bS;	
	  //uint * bits_bitmap;
	  uint * base;
	  ushort * base_bits;
	  uint * tablebase;
	  uint tamtablebase;

  	
} FTRepI;



// public:
	FTRepI * createFTI(uint *list,uint listLength);
	uint accessFTI(FTRepI * listRep,uint param);
	void saveFTI(FTRepI * listRep, FILE * flist);
	uint * decompressFTI(FTRepI * listRep, uint n);
	FTRepI * loadFTI(FILE * flist);
	void destroyFTI(FTRepI * listRep);

#endif

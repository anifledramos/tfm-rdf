/* Wavelet Tree over Dense Code. -- 
A word index using wavelet tree strategy over compressed text.

Programmed by Susana Ladra.

Author's contact: Susana Ladra, Databases Lab, University of
A Coruna. Campus de Elvina s/n. Spain  sladra@udc.es

Copyright (C) 2007  Nieves R. Brisaboa, Antonio Farina and Susana Ladra
Author's contact: susanaladra@gmail.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
  


/*-----------------------------------------------------------------------
 Hash: Definition of HashTable class (Linear Hash)
 ------------------------------------------------------------------------*/
#ifndef __DAC__
#define __DAC__
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#include "mymalloc.h"
#include "basic.h"
#include "bitrankw32int.h"

typedef struct sFTRep {
	  uint listLength;
	  byte nLevels;
	  byte * levels;
	  uint * levelsIndex;
	  uint * rankLevels;
	  bitRankW32Int * bS;	
	  uint * bits_bitmap;
} FTRep;



	// public:
	FTRep* createFT(uint *list,uint listLength);
	uint accessFT(FTRep * listRep,uint param);
	void saveFT(FTRep * listRep, FILE * f);
	uint * decompressFT(FTRep * listRep, uint n);
	FTRep* loadFT(FILE * f);
	void destroyFT(FTRep * listRep);


#endif

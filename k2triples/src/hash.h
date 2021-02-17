/* Searches using  Horspool's algorithm adapted to
search inside a text compressed with End-Tagged Dense Code.
Lightweight Natural Language Text Compression: Information Retrieval 2006

Programmed by Antonio Fari�a.

Author's contact: Antonio Fari�a, Databases Lab, University of
A Coru�a. Campus de Elvi�a s/n. Spain  fari@udc.es

Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fari�a and Jos� R. Param�
Author's contact: antonio.fari@gmail.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
aint with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


/*-----------------------------------------------------------------------
 Hash.h: Definition of a HashTable that uses linear hash
 ------------------------------------------------------------------------*/

#ifndef __HASH_H
#define __HASH_H
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#include "MemoryManager.h"

#define JUMP 101  		 //jump done when a collision appears
#define OCUP_HASH 1.5	 	 //index of occupation of the hash table
#define SMALL_PRIME 1009 // a small prime number, used to compute a hash function
#define SEED	1159241
/* Type definitions */

#define MIN(a,b) (a < b) ? a : b


struct Nword {
	  unsigned char *word;
	  unsigned int size;
	  unsigned int len;
	  unsigned int weight;
	  unsigned int codeword;
};

typedef struct Nword t_word;

// private:

	MemoryManager _memMgr; 	  /* Holds dynamic memory reserve for words. */

	unsigned int TAM_HASH;   /* # entries in the hash table    */
	unsigned int NumElem;    /* # elements already added to the hash table*/

	unsigned int initialize_hash (unsigned int tamArq);
	unsigned int NearestPrime(unsigned int n);
	unsigned int hashFunction (const unsigned char *aWord, unsigned int len);

	int strcomp(const unsigned char *s1, const unsigned char *s2, register unsigned int ws1, unsigned int ws2);

// public:
	t_word  *hash;     		  /* holds a hashTable of words  */

	unsigned int initialize_hash  (unsigned int tamArq);
	void freeHashTable();
	unsigned int insertElement (const unsigned char *aWord, register unsigned int len,
										register unsigned int *addr);
	unsigned int search (const unsigned char *aWord, register unsigned len,
									unsigned int *returnedAddr);


#endif

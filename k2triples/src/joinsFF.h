#ifndef ___JOINSFF
#define ___JOINSFF

#include "kTree.h"
#include "commons.h"
#include "merge.h"
#include "interfazSO.h"
#include "kTreeJoinInt.h"
//JOIN 1 - SPV VPO

int sppo(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2, int parte1, int parte2);

int sppoParalelo(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2, int parte1, int parte2);

int sppoIzquierda(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2, int parte1,
		int parte2);

int sppoDerecha(TREP ** treps, int nPreds, int s1, int p1, int p2, int o2, int parte1, int parte2);

//JOIN 2 - SPV VVO 

int sp_o(TREP ** treps, int nPreds, INDICEDAC   sp, INDICEDAC   op, int s1, int p1, int o2, int condac,
		int parte1, int parte2);

int sp_oInterseccion(TREP ** treps, int nPreds, INDICEDAC  sp, INDICEDAC  op, int s1, int p1, int o2,
		int condac, int parte1, int parte2);

int sp_oParalelo(TREP ** treps, int nPreds, INDICEDAC  sp, INDICEDAC  op, int s1, int p1, int o2,
		int condac, int parte1, int parte2);

//JOIN 3 - svv vvo

int s__o(TREP ** treps, int nPreds, INDICEDAC  sp, INDICEDAC  op, int s1, int o2, int condac,
		int parte1, int parte2);

int s__oParalelo(TREP ** treps, INDICEDAC  sp, INDICEDAC op, int nPreds, int s1, int o2, int condac,
		int parte1, int parte2);

int s__oMerge(TREP ** treps, int numPreds, INDICEDAC  sp, INDICEDAC   op, int sujeto, int objeto2,
		int condac, int parte1, int parte2);

int s__oMergeParalelo(TREP ** treps, int numPreds, INDICEDAC  sp, INDICEDAC  op, int sujeto,
		int objeto2, int condac, int parte1, int parte2);

#endif

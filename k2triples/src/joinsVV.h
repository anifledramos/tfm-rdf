#ifndef ___JOINSVV
#define ___JOINSVV

#include "kTree.h"
#include "kTreeJoinInt.h"
#include "commons.h"
#include "merge.h"
#include "interfazSO.h"

//JOIN 7 - VPV VPV

int _pp_Izquierda(TREP ** treps, int p1, int p2, int parte1, int parte2);

int _pp_Derecha(TREP ** treps, int p1, int p2, int parte1, int parte2);

int _pp_Interactiva(TREP ** treps, int p1, int p2, int parte1, int parte2);

int _pp_Independiente(TREP ** treps, int p1, int p2, int parte1, int parte2);

//JOIN 8 - VPV VVV
int _p__Izquierda(TREP ** treps, int p1, int nPreds, INDICEDAC sp, INDICEDAC op, int condac,
		int parte1, int parte2);

int _p__Interactiva(TREP ** treps, int p1, int nPreds, int parte1, int parte2);

#endif

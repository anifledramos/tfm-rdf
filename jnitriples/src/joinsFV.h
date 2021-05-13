#ifndef ___JOINSFV
#define ___JOINSFV

#include "kTree.h"
#include "commons.h"
#include "merge.h"
#include "interfazSO.h"
#include "kTreeJoinInt.h"
//JOIN 4 - SPV VPO
int _ppoDerecha(TREP ** treps, int p1, int p2, int o2, int parte1, int parte2);

int _ppoInteractiva(TREP ** treps, int p1, int p2, int o2, int parte1, int parte2);

int _ppoIndependiente(TREP ** treps, int p1, int p2, int o2, int parte1, int parte2);



//JOIN 5
int _p_oIzquierda(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int p1, int o2, int condac,
		int parte1, int parte2);

int _p_oDerecha(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int p1, int o2, int condac,
		int parte1, int parte2);

int _p_oInteractiva(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int p1, int o2,
		int condac, int parte1, int parte2);

int _p_oIndependiente(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int p1, int o2,
		int condac, int parte1, int parte2);

int _p_oMergeIndependiente(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op, int p1, int o2,
		int condac, int parte1, int parte2);

//JOIN 52
int __poDerecha(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op,
		int s1,int p1, int parte1, int parte2);

int __poInteractiva(TREP ** treps, int nPreds, INDICEDAC sp, INDICEDAC op,
		int s1,int p1, int parte1, int parte2);


//JOIN 6
int ___oDerecha(TREP ** treps, int nPreds, INDICEDAC indiceSP, INDICEDAC indiceOP, int o2,
		int condac, int parte1, int parte2);

int ___oInteractiva(TREP ** treps, int nPreds, INDICEDAC indiceSP, INDICEDAC indiceOP, int o2,
		int condac, int parte1, int parte2);

#endif

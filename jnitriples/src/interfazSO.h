#ifndef ___INTERFAZSO
#define ___INTERFAZSO
#include <stdio.h>
#include "kTree.h"
#include "commons.h"

int INVERTIDO;


inline void setInvertido(int inv);

/* EVALUACIÓN EN CADENA: OPERACIONES BÁSICAS */
inline int compactNeighbour(TREP * trep, int y, uint * resultados, int parte);

inline int compactNeighbourOpposite(TREP * trep, int y, uint * resultados, int parte);

inline int compactNeighbourMerge(TREP * trep,int x, Result2 **  vectores, int indiceVector, int predicado, int parte);

inline int compactLink(TREP * trep, int x, int y, int parte);

inline int obtenerPar(SET par,int parte);

inline int obtenerOppositePar(SET par,int parte);

// dac
inline int commonObtenerPredicados(INDICEDAC sp, INDICEDAC op, int indice, int parte);

inline int commonObtenerPredicados2(INDICEDAC sp, INDICEDAC op, int indice, int parte);



/* EVALUACIÓN EN PARALELO */
inline MREP * obtenerRep(TREP * trep, int x, int y, int parte);

inline int getNode(int K,int i,int div_level,int indexRel,int parteDer);

inline int getNodeXY(int K, int x, int y, int indexRel, int parteDer);

inline void commonAddItem2(TREP * trepIndex, int rankValue, int i,
		int div_level, int indexRel, int flag, int parte);

inline void CommonInitItemJoin2(TREP * trepIndex, int elem,int x,int y, int pred, int flag,int parte);

inline void CommonInitItemJoin(TREP * trepIndex, int elem,int x,int y, int pred,int parte);

inline void commonAddItem(TREP * trepIndex, int rankValue, int i, int div_level, int indexRel,
		 int parte);

inline void commonAddItemXY(TREP * trepIndex, int rankValue, int x,int y, int div_level, int indexRel,
		int parte);

inline void commonAddItemXY2(TREP * trepIndex, int rankValue, int x,int y, int div_level, int indexRel,
		int flag,int parte);

inline int getPosRel(int i,int x,int K, int parteDer);

inline int getPosRelXY(int x, int y, int K, int parteDer);

inline int getPosAbs(int K, int i, int j, int indexRel,int indiceRel, int tamSubm, int parte);

inline int getPosAbs2(int K, int i, int j, int indexRel, int indiceRel, int tamSubm, int parte);

inline int elegirDimension(int i,int j,int parte);

/* DEBUG */
// void printTripleta(int x,int y,int z,int parte);

inline void printCompleto(int x1,int y1,int z1,int parte1, int x2,int y2,int z2,int parte2);

#endif

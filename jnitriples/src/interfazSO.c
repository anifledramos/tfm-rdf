#include "interfazSO.h"

inline void setInvertido(int inv){
	INVERTIDO=inv;
}


/* EVALUACIÓN EN CADENA: OPERACIONES BÁSICAS */
inline int compactNeighbour(TREP * trep, int y, uint * resultados, int parte) {

	if (parte == PORSUJETO) {
		return compactTreeInverseList(trep, y, resultados);
	} else {
		return compactTreeAdjacencyList(trep, y, resultados);
	}

}

inline int compactNeighbourOpposite(TREP * trep, int y, uint * resultados, int parte) {

	if (parte == POROBJETO) {
		return compactTreeInverseList(trep, y, resultados);
	} else {
		return compactTreeAdjacencyList(trep, y, resultados);
	}

}

inline int compactNeighbourMerge(TREP * trep,int x, Result2 **  vectores, int indiceVector, int predicado, int parte){
	if (parte == PORSUJETO) {
		return compactTreeInverseListMerge(trep, x,vectores,indiceVector,predicado);
	} else {
		return compactTreeAdjacencyListMerge(trep, x,vectores,indiceVector,predicado);
	}
}


inline int compactLink(TREP * trep, int x, int y, int parte) {

	if (parte == PORSUJETO) {
		return compactTreeCheckLinkQuery(trep, x, y);
	} else {
		return compactTreeCheckLinkQuery(trep, y, x);
	}

}

inline int obtenerPar(SET par,int parte){
	if (parte == PORSUJETO) {
		return par.x;
	} else {
		return par.y;
	}
}

inline int obtenerOppositePar(SET par,int parte){
	if (parte == POROBJETO) {
		return par.x;
	} else {
		return par.y;
	}
}
// dac
//TODO: pasar resDAC por parametro
inline int commonObtenerPredicados(INDICEDAC sp, INDICEDAC op, int indice, int parte){
	if (parte==PORSUJETO)
		return obtenerPredicados(op,indice);
	else
		return obtenerPredicados(sp,indice);
}

inline int commonObtenerPredicados2(INDICEDAC sp, INDICEDAC op, int indice, int parte){
	if (parte==PORSUJETO)
		return obtenerPredicados2(op,indice);
	else
		return obtenerPredicados2(sp,indice);
}

/* EVALUACIÓN EN PARALELO */
inline MREP * obtenerRep(TREP * trep, int x, int y, int parte) {
	if (parte == PORSUJETO) {
		return trep->submatrices[y][x];
	} else {
		return trep->submatrices[x][y];
	}
}

inline int getNode(int K, int i, int div_level, int indexRel, int parteDer) {

	if (parteDer == PORSUJETO) {
		//Vecino inverso
		return K * i + basex[indexRel] / div_level + element[indexRel];
	} else {
		//Vecino directo
		return basey[indexRel] / div_level * K + i + element[indexRel];
	}
}

inline int getNodeXY(int K, int x, int y, int indexRel, int parteDer) {

	if (parteDer == PORSUJETO) {
		//Vecino inverso
		return K * y + x + element[indexRel];
	} else {
		//Vecino directo
		return K * x + y + element[indexRel];
	}
}

inline void CommonInitItemJoin2(TREP * trepIndex, int elem,int x,int y, int pred, int flag,int parte) {

	if (parte==PORSUJETO) {
		AddItemJoin2(trepIndex,elem,x,y,pred,flag);
	}
	else {
		AddItemJoin2(trepIndex,elem,y,x,pred,flag);
	}
}

inline void commonAddItem2(TREP * trepIndex, int rankValue, int i, int div_level, int indexRel,
		int flag, int parte) {

	if (parte == PORSUJETO) {
		AddItemJoin2(trepIndex, rankValue, basex[getPQ(indexRel)] % div_level,
				basey[getPQ(indexRel)] + i * div_level, indicesRep[getPQ(indexRel)], flag);
	} else {
		AddItemJoin2(trepIndex, rankValue, basex[getPQ(indexRel)] + i * div_level,
				basey[getPQ(indexRel)] % div_level, indicesRep[getPQ(indexRel)], flag);
	}

}


// AddItem es igual que AddItem2, pero sin flag
inline void CommonInitItemJoin(TREP * trepIndex, int elem,int x,int y, int pred,int parte) {



	if (parte==PORSUJETO) {
		AddItemJoin(trepIndex,elem,x,y,pred);
	}
	else {
		AddItemJoin(trepIndex,elem,y,x,pred);
	}
}

inline void commonAddItem(TREP * trepIndex, int rankValue, int i, int div_level, int indexRel,
		int parte) {

	if (parte == PORSUJETO) {
		AddItemJoin(trepIndex, rankValue, basex[getPQ(indexRel)] % div_level,
				basey[getPQ(indexRel)] + i * div_level, indicesRep[getPQ(indexRel)]);
	} else {
		AddItemJoin(trepIndex, rankValue, basex[getPQ(indexRel)] + i * div_level,
				basey[getPQ(indexRel)] % div_level, indicesRep[getPQ(indexRel)]);
	}

}

inline void commonAddItemXY(TREP * trepIndex, int rankValue, int x,int y, int div_level, int indexRel,
		int parte) {

	if (parte == PORSUJETO) {
		AddItemJoin(trepIndex, rankValue, basex[getPQ(indexRel)] + div_level * y,
				basey[getPQ(indexRel)] + div_level*x, indicesRep[getPQ(indexRel)]);
	} else {
		AddItemJoin(trepIndex, rankValue, basex[getPQ(indexRel)] + div_level * x,
				basey[getPQ(indexRel)] + div_level*y, indicesRep[getPQ(indexRel)]);
	}

}

inline void commonAddItemXY2(TREP * trepIndex, int rankValue, int x,int y, int div_level, int indexRel,
		int flag,int parte) {

	if (parte == PORSUJETO) {
		AddItemJoin2(trepIndex, rankValue, basex[getPQ(indexRel)] + div_level * y,
				basey[getPQ(indexRel)] + div_level*x, indicesRep[getPQ(indexRel)],flag);
	} else {
		AddItemJoin2(trepIndex, rankValue, basex[getPQ(indexRel)] + div_level * x,
				basey[getPQ(indexRel)] + div_level*y, indicesRep[getPQ(indexRel)],flag);
	}

}

inline int getPosRel(int i, int x, int K, int parteDer) {

	if (parteDer == PORSUJETO) {
		return i * K + (x % K);
	} else {
		return i + (x % K) * K;
	}

}

inline int getPosRelXY(int x, int y, int K, int parteDer) {

		if (parteDer == PORSUJETO) {
			return (y%K)*K + (x%K);
		} else {
			return (x%K)*K + (y%K);
		}

	}

inline int getPosAbs(int K, int i, int j, int indexRel, int indiceRel, int tamSubm, int parte) {

	if (parte == PORSUJETO) {
		return basey[indexRel] + i + K * j + indiceRel * tamSubm;
	} else {
		return basex[indexRel] + i + K * j + indiceRel * tamSubm;
	}

}


inline int getPosAbs2(int K, int i, int j, int indexRel, int indiceRel, int tamSubm, int parte) {

	if (parte == PORSUJETO) {
		return basex[indexRel] + i + K * j + indiceRel * tamSubm;
	} else {
		return basey[indexRel] + i + K * j + indiceRel * tamSubm;
	}

}

inline int elegirDimension(int i,int j,int parte){
	if (parte == PORSUJETO) {
		return j;
	} else {
		return i;
	}

}

/* DEBUG */
inline void printTripleta(int x, int y, int z, int parte) {
	if (foutput==NULL){
		if (parte == PORSUJETO) {
			fprintf(stdout, "<%d,%d,%d>", z, y, x);
		} else {
			fprintf(stdout, "<%d,%d,%d>", x, y, z);
		}
	}
	else{
		if (parte == PORSUJETO) {
			fprintf(foutput, "%d %d %d ", z, y, x);
		} else {
			fprintf(foutput, "%d %d %d ", x, y, z);
		}
	}	
}


inline void printCompleto(int x1,int y1,int z1,int parte1,int x2,int y2,int z2,int parte2){

	if (INVERTIDO){
		printTripleta(x2,y2,z2,parte2);
		printTripleta(x1,y1,z1,parte1);			
	}
	else{
		printTripleta(x1,y1,z1,parte1);
		printTripleta(x2,y2,z2,parte2);
	}

	if (foutput==NULL){
		fprintf(stdout,"\n");
	}
	else{
		fprintf(foutput,"\n");	
	}

}


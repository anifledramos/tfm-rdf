/*
 * kTreeCreation.c
 *
 *  Created on: 09/02/2012
 *      Author: sandra
 */

#include "kTreeCreation.h"

/*------------------------------------------------------------------
 Initilizes the structures used.
 ---------------------------------------------------------------- */
void initialize(unsigned int sizeVoc) {
	//unsigned long i;

	_memMgr = createMemoryManager();
	initialize_hash(sizeVoc);

	positionInTH = (unsigned int*) mymalloc(sizeVoc * sizeof(unsigned int));
	zeroNode = 0;
}

NODE * createKTree(int _K1, int _K2, int maxreallevel1, int maxlevel1,
		int maxlevel2) {
	NODE * n = (NODE *) mymalloc(sizeof(struct node));
	n->child = NULL;
	n->data = 0;
	//	K1 = _K1;
	//	K2 = _K2;
	max_real_level1 = maxreallevel1;
	max_Level1 = maxlevel1;
	max_Level2 = maxlevel2 - L;
	numberNodes = 0;
	numberLeaves = 0;
	numberTotalLeaves = 0;
	return n;
}

MREP * loadRepresentation(char * basename) {
	MREP * rep;
	rep = (MREP *) mymalloc(sizeof(struct matrixRep));
	rep->bt = (bitRankW32Int *) mymalloc(sizeof(struct sbitRankW32Int));
	rep->bn = (bitRankW32Int *) mymalloc(sizeof(struct sbitRankW32Int));

	fprintf(stderr, "Recovering tree bitmap\n");
	char *filename = (char *) mymalloc(sizeof(char) * (strlen(basename) + 4));
	strcpy(filename, basename);
	strcat(filename, ".tr");
	FILE * ft = fopen(filename, "r");
	load(rep->bt, ft);
	fclose(ft);
	rep->bt_len = rep->bt->n;
	fprintf(stderr, "Bitmap recovered (len: %d bits)\n", rep->bt_len);

	fprintf(stderr, "Recovering leaves bitmap\n");
	strcpy(filename, basename);
	strcat(filename, ".lv");
	FILE * fn = fopen(filename, "r");
	load(rep->bn, fn);
	fclose(fn);
	rep->bn_len = rep->bn->n;
	fprintf(stderr, "Bitmap recovered (len: %d bits)\n", rep->bn_len);

	fprintf(stderr, "Recovering leaves information\n");
	strcpy(filename, basename);
	strcat(filename, ".il");
	FILE * fi = fopen(filename, "r");
	if (1!=fread(&(rep->numberOfNodes), sizeof(uint), 1, fi)){
		fprintf(stderr,"loadTreeRep:error de lectura\n");
		return NULL;
	}

	fprintf(stderr, "Number of nodes : %d\n", rep->numberOfNodes);
	if (1!=fread(&(rep->numberOfEdges), sizeof(uint), 1, fi))
		{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}

	fprintf(stderr, "Number of edges : %d\n", rep->numberOfEdges);

	if (1!=fread(&(rep->cutBt), sizeof(uint), 1, fi))
		{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
	fprintf(stderr, "cutBt : %d\n", rep->cutBt);

	if (1!=fread(&(rep->lastBt1_len), sizeof(uint), 1, fi))
		{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
	fprintf(stderr, "lastBt1_len : %d\n", rep->lastBt1_len);
	if (1!=fread(&(rep->nleaves), sizeof(uint), 1, fi))
		{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
	fprintf(stderr, "Leaves to read: %d\n", rep->nleaves);
	rep->leavesInf = (uint *) mymalloc(
			sizeof(uint) * (rep->nleaves * K2 * K2 / W + 1));
	if (  (rep->nleaves * K2 * K2 / W + 1)!= fread(rep->leavesInf, sizeof(uint), rep->nleaves * K2 * K2 / W + 1, fi)){
		fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;
	}
	fclose(fi);

	fprintf(stderr, "Leaves information recovered (len: %d bits)\n",
			rep->nleaves * K2 * K2);
	free(filename);
	return rep;
}

void destroyRepresentation(MREP * rep) {
	destroyBitRankW32Int(rep->bt);
	destroyBitRankW32Int(rep->bn);
	destroyFT(rep->compressIL);
	if (rep->leavesInf != NULL)
		free(rep->leavesInf);
	free(rep);
}



MREP * createRepresentation(NODE * root, uint numberOfNodes, uint numberOfEdges) {
	MREP * rep;
	rep = mymalloc(sizeof(struct matrixRep));
	//trep->maxRealLevel1 = max_real_level1;
	//trep->maxLevel1 = max_Level1;
	//trep->maxLevel2 = max_Level2;
	//rep-> K1 = K1;
	//rep-> K2 = K2;
	rep->numberOfNodes = numberOfNodes;
	rep->numberOfEdges = numberOfEdges;
	uint bits_BT_len = numberNodes;
	uint bits_BN_len = numberTotalLeaves;
	uint bits_LI_len = numberLeaves * K2_2 * K2_2;
	bitRankW32Int *BT, *BN;
	uint * bits_BT = (uint*) mymalloc(sizeof(uint) * ((bits_BT_len + W - 1) / W));
	uint * bits_BN = (uint*) mymalloc(sizeof(uint) * ((bits_BN_len + W - 1) / W));
	uint * bits_LI = (uint*) mymalloc(sizeof(uint) * ((bits_LI_len + W - 1) / W));
	int i, k, j, queuecont, conttmp, node, div_level, pos = 0;
	//fprintf(stderr,"numberNodes = %d\n.... long entero: %d",numberNodes,(bits_BT_len+W-1)/W);
	for (i = 0; i < (W - 1 + bits_BT_len) / W; i++)
		bits_BT[i] = 0;
	for (i = 0; i < (W - 1 + bits_BN_len) / W; i++)
		bits_BN[i] = 0;
	for (i = 0; i < (W - 1 + bits_LI_len) / W; i++)
		bits_LI[i] = 0;

	char isroot = 1;
	QUEUECONS * q = NULL;
	finalQUEUECONS = q;
	q = AddItemCONS(q, root);
	queuecont = 1;
	for (i = 0; i < max_Level1; i++) {
		conttmp = 0;
		div_level = exp_pow(K1, max_real_level1 - i);
		//div_level_x = x/div_level*K1;
		//		printf("Nivel: %d, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for (k = 0; k < queuecont; k++) {
			//	fprintf(stderr,"Nivel: %d, div_level %d, queuecont %d k %d\n",i,div_level,queuecont, k);

			if (q->element->child != NULL) {
				for (j = 0; j < K1 * K1; j++) {
					node = j;
					//fprintf(stderr,"node: %d pos %d\n",node,pos);
					conttmp++;
					//	fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",q->basex+j*div_level,q->basey+xrelat/div_level*div_level);
					q = AddItemCONS(q, q->element->child[node]);

				}
				if (!isroot)
					bitset(bits_BT,pos);

			}
			if (!isroot)
				pos++;
			isroot = 0;
			//fprintf(stderr,"Contenido bitmap:  ");
			//for(j=0; j<pos;j++)
			//	fprintf(stderr,"%x",bitget(bits_BT,j));
			//fprintf(stderr,"\n");
			free(q->element);
			q = (QUEUECONS *) RemoveItemCONS(q);
		}
		queuecont = conttmp;
	}
	//fprintf(stderr,"pos para cutBt: %d lastBt1_len %d\n",pos+queuecont,pos);
	rep->lastBt1_len = pos;
	rep->cutBt = pos + queuecont;
	for (i = 0; i < max_Level2; i++) {
		conttmp = 0;
		div_level = exp_pow(K2, max_Level2 + L - i);
		//div_level_x = x/div_level*K1;
		//	printf("Nivel: %d, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for (k = 0; k < queuecont; k++) {
			//	fprintf(stderr,"Nivel: %d, div_level %d, queuecont %d k %d\n",i,div_level,queuecont, k);

			if (q->element->child != NULL) {
				for (j = 0; j < K2 * K2; j++) {
					node = j;
					//		fprintf(stderr,"node: %d\n",node);
					conttmp++;
					//	fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",q->basex+j*div_level,q->basey+xrelat/div_level*div_level);
					q = AddItemCONS(q, q->element->child[node]);

				}
				if (!isroot)
					bitset(bits_BT,pos);
				free(q->element->child);

			}
			pos++;
			//fprintf(stderr,"Contenido bitmap:  ");
			//for(j=0; j<pos;j++)
			//	fprintf(stderr,"%x",bitget(bits_BT,j));
			//fprintf(stderr,"\n");

			q = (QUEUECONS *) RemoveItemCONS(q);
		}
		queuecont = conttmp;
	}

	BT = createBitRankW32Int(bits_BT, bits_BT_len, 1, 20);

	rep->bt = BT;
	rep->bt_len = pos;

	pos = 0;
	uint pos_inf = 0;
	//	fprintf(stderr,"Empezando bitmap de hojas utiles\n");
	while (q != NULL) {
		//		fprintf(stderr,"quecont: %d\n Datos: %x\n",queuecont,q->element->data);
		if (hasAnyBitSet((q->element)->data)) {
			//			fprintf(stderr,"poniendo a uno el bit %d\n",pos);
			bitset(bits_BN,pos);

			for (i = 0; i < K2_2 * K2_2; i++) {
				//fprintf(stderr,"probando %d, %x %x %x\n",i+(x%K1)*K1,(q->element)->data,(0x1<<(i+(x%K1)*K1)),((q->element)->data)&(0x1<<(i+(x%K1)*K1)));
				if (bitgetchar((q->element)->data,i)) {
					bitset(bits_LI,pos_inf);
				}
				pos_inf++;
			}
		}
		pos++;
		//			fprintf(stderr,"Contenido bitmap:  ");
		//			for(i=0; i<pos;i++)
		//				fprintf(stderr,"%x",bitget(bits_BN,i));
		//			fprintf(stderr,"\n");
		free(q->element);
		q = (QUEUECONS *) RemoveItemCONS(q);
	}
	//			fprintf(stderr,"Contenido bitmap BN (long: %d):  ", pos);
	//		for(i=0; i<pos;i++)
	//			fprintf(stderr,"%x",bitget(bits_BN,i));
	//		fprintf(stderr,"\n");
	//
	//
	//			fprintf(stderr,"Contenido bitmap LI (long: %d):  ", pos_inf);
	//		for(i=0; i<pos_inf;i++)
	//			fprintf(stderr,"%x",bitget(bits_LI,i));
	//		fprintf(stderr,"\n");

	BN = createBitRankW32Int(bits_BN, bits_BN_len, 1, 20);

	rep->bn = BN;
	rep->bn_len = pos;

	rep->leavesInf = bits_LI;
	rep->nleaves = numberLeaves;

	//printf("Representacion:\n%d bits para arbol\n%d bits para hojas\n%d bits hojas utiles\n", rep->bt_len, rep->bn_len, rep->nleaves*K2*K2);
	return rep;
}

void insertNode(NODE * root, int x, int y) {
	//fprintf(stderr,"MaxLevel1: %d MaxLevel2 %d K1: %d K2 %d\n",max_Level1,max_Level2,K1,K2);
	uint i, node, div_level;
	int l = 0;
	NODE * n = root;
	//fprintf(stderr,"\nEntrando x: %d y: %d\n",x,y);
	while (l < max_Level1) {
		//printf("Level %d de K1, x: %d y %d\n",l,x,y);
		div_level = exp_pow(K1, max_real_level1 - l);
		//fprintf(stderr,"x/div_level = %d, y/div_level = %d, %d, %d\n",(x / div_level),y /div_level,(x / div_level)*K1,(x / div_level)*K1+y /div_level);
		node = (x / div_level) * K1 + y / div_level;
		if (n->child == NULL) {
			//fprintf(stderr,"No existian hijos, creando los hijos");
			//if(l<max_Level1-1)
			numberNodes += K1 * K1;
			//else
			//	numberTotalLeaves+=K1*K1;
			//fprintf(stderr,"number nodes: %d\n",numberNodes);
			n->child = (NODE **) mymalloc(sizeof(NODE *) * K1 * K1);
			for (i = 0; i < K1 * K1; i++) {
				n->child[i] = (NODE *) mymalloc(sizeof(struct node));
				n->child[i]->child = NULL;
				n->child[i]->data = 0;
			}
		}
		//fprintf(stderr,"Yendo al hijo %d\n",node);
		n = n->child[node];

		//fprintf(stderr,"bucle: x: %d, y: %d... node %d\n",x,y,node);
		x = x % div_level;
		y = y % div_level;
		l++;
	}

	l = 0;
	int j;
	while (l <= max_Level2) {
		//fprintf(stderr,"MaxLevel1: %d MaxLevel2 %d K1: %d K2 %d\n",max_Level1,max_Level2,K1,K2);
		//fprintf(stderr,"Level %d de K2, x: %d y %d\n",l,x,y);
		div_level = exp_pow(K2, max_Level2 + L - l);
		//fprintf(stderr,"x/div_level = %d, y/div_level = %d, %d, %d\n",(x / div_level),y /div_level,(x / div_level)*K2,(x / div_level)*K2+y /div_level);
		node = (x / div_level) * K2 + y / div_level;
		if (l == max_Level2) {
			//fprintf(stderr,"Dentro de la hoja node: %d\n",node);
			if (!hasAnyBitSet(n->data)) {
				numberLeaves++;
				/*
				 //	fprintf(stderr,"Creando la hoja\n");
				 n->data=(char *)mymalloc(sizeof(char)*K1*K1);
				 for(i=0;i<K1*K1;i++)
				 n->data[i]=0;*/
			}
			node = x * K2_2 + y;
			bitsetchar(n->data,node);
			//fprintf(stderr,"Introduciendo un 1 en la hoja %d: %x\n",node,n->data);
			//n->data[node]=1;
		} else {
			if (n->child == NULL) {
				//fprintf(stderr,"No existian hijos, creando los hijos");
				if (l < max_Level2 - 1)
					numberNodes += K2 * K2;
				else
					numberTotalLeaves += K2 * K2;
				//fprintf(stderr,"number nodes: %d\n",numberNodes);
				n->child = (NODE **) mymalloc(sizeof(NODE *) * K2 * K2);
				for (i = 0; i < K2 * K2; i++) {
					n->child[i] = (NODE *) mymalloc(sizeof(struct node));
					n->child[i]->child = NULL;

					if (l == max_Level2 - 1) {
						n->child[i]->data = mymalloc(sizeof(char) * K2_3_char);
						//fprintf(stderr,"haciendo un mymalloc de %d bytes, pos: %x\n",K_2/8,n->child[i]->data);
						for (j = 0; j < K2_3_char; j++)
							n->child[i]->data[j] = 0;
					}

				}
			}
			//fprintf(stderr,"Yendo al hijo %d\n",node);
			n = n->child[node];

		}

		//fprintf(stderr,"bucle: x: %d, y: %d... node %d\n",x,y,node);
		x = x % div_level;
		y = y % div_level;
		l++;
	}

}

TREP * createTreeRep(uint nodesOrig, uint edges, uint part, uint subm,
		uint max_real_level1, uint max_level1, uint max_level2, uint _K1,
		uint _K2) {
	TREP * trep;
	trep = mymalloc(sizeof(struct treeRep));
	trep->part = part;
	trep->tamSubm = subm;
	trep->numberOfNodes = nodesOrig;
	trep->numberOfEdges = edges;
	trep->maxRealLevel1 = max_real_level1;//-L;
	trep->maxLevel1 = max_level1;
	trep->maxLevel2 = max_level2 - L;
	trep->repK1 = _K1;
	trep->repK2 = _K2;

	trep->submatrices = (MREP ***) mymalloc(sizeof(MREP **) * part);
	int i;
	for (i = 0; i < part; i++) {
		trep->submatrices[i] = (MREP **) mymalloc(sizeof(MREP *) * part);
	}

	//		trep->info = (uint *)mymalloc(sizeof(uint)*MAX_INFO);

	//		trep->info2[0] = (uint *)mymalloc(sizeof(uint)*MAX_INFO);

	//		trep->info2[1] = (uint *)mymalloc(sizeof(uint)*MAX_INFO);

	//element = (uint *)mymalloc(sizeof(uint)*MAX_INFO);

	//basex = (uint *)mymalloc(sizeof(uint)*MAX_INFO);

	//basey = (uint *)mymalloc(sizeof(uint)*MAX_INFO);
	//basep1 = (uint *)mymalloc(sizeof(uint)*MAX_INFO);
	//  basep2 = (uint *)mymalloc(sizeof(uint)*MAX_INFO);
	//  baseq1 = (uint *)mymalloc(sizeof(uint)*MAX_INFO);
	//  baseq2 = (uint *)mymalloc(sizeof(uint)*MAX_INFO);
	trep->iniq = -1;
	trep->finq = -1;
	trep->div_level_table1 = (uint *) mymalloc(sizeof(uint) * trep->maxLevel1);
	for (i = 0; i < trep->maxLevel1; i++) {
		trep->div_level_table1[i] = exp_pow(K1, trep->maxRealLevel1 - i);
	}

	trep->div_level_table2 = (uint *) mymalloc(sizeof(uint) * trep->maxLevel2);
	for (i = 0; i < trep->maxLevel2; i++) {
		trep->div_level_table2[i] = exp_pow(K2, trep->maxLevel2 + L - i);
	}

	return trep;

}

void insertIntoTreeRep(TREP * trep, MREP * rep, uint i, uint j) {
	trep->submatrices[i][j] = rep;

}


TREP * loadTreeRepresentation(FILE * file) {
  TREP * trep=(TREP *) mymalloc(sizeof(TREP));
  MREP * rep;
  int i;
  trep = mymalloc(sizeof(struct treeRep));

  FILE * fv = file;

  if (1!=fread(&(trep->part), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->tamSubm), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->numberOfNodes), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->numberOfEdges), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->repK1), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->repK2), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->maxRealLevel1), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->maxLevel1), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->maxLevel2), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&trep->zeroNode, sizeof(uint), 1, fv)) //reads the number of words of the vocabulary
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&trep->lenWords, sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  trep->words = (unsigned char *) mymalloc(
      sizeof(unsigned char) * trep->lenWords * trep->zeroNode);
  if (trep->lenWords*trep->zeroNode!=fread(trep->words, sizeof(unsigned char), trep->lenWords * trep->zeroNode,
      fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}

  trep->div_level_table1 = (uint *) mymalloc(sizeof(uint) * trep->maxLevel1);
  for (i = 0; i < trep->maxLevel1; i++)
    trep->div_level_table1[i] = exp_pow(K1, trep->maxRealLevel1 - i);

  trep->div_level_table2 = (uint *) mymalloc(sizeof(uint) * trep->maxLevel2);
  for (i = 0; i < trep->maxLevel2; i++)
    trep->div_level_table2[i] = exp_pow(K2, trep->maxLevel2 + L - i);
  trep->iniq = -1;
  trep->finq = -1;

  trep->submatrices = (MREP ***) mymalloc(sizeof(MREP **) * trep->part);

  int fila, columna;
  for (i = 0; i < trep->part; i++) {
    trep->submatrices[i] = (MREP **) mymalloc(sizeof(MREP *) * trep->part);
  }
        int numBytes=getMemoria();
        numBytes=getMemoria();

  FILE * fi = file;

  int totalnumberLeaves = 0;
  
  for (fila = 0; fila < trep->part; fila++) {
    for (columna = 0; columna < trep->part; columna++) {
      int numberOfNodes,numberOfEdges;
      if (1!=fread(&(numberOfNodes), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}

      if (1!=fread(&(numberOfEdges), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}



      if (numberOfEdges == 0){
        trep->submatrices[fila][columna]=NULL;
        continue;
      }
      rep = (MREP *) mymalloc(sizeof(struct matrixRep));

      rep->bt = (bitRankW32Int *) mymalloc(
          sizeof(struct sbitRankW32Int));

      rep->bn = (bitRankW32Int *) mymalloc(sizeof(struct sbitRankW32Int));
      trep->submatrices[fila][columna] = rep;

      rep->numberOfNodes=numberOfNodes;
      rep->numberOfEdges=numberOfEdges;

      if (1!=fread(&(rep->cutBt), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
      if (1!=fread(&(rep->lastBt1_len), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
      if (1!=fread(&(rep->nleaves), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}

      rep->compressIL = loadFT(fi);
      totalnumberLeaves += rep->compressIL->listLength;
      rep->leavesInf = NULL;

    }
  }
  FILE * ft = file;
  for (fila = 0; fila < trep->part; fila++) {
    for (columna = 0; columna < trep->part; columna++) {
      rep = trep->submatrices[fila][columna];
      if (rep == NULL)
        continue;
      load(rep->bt, ft);
      rep->bt_len = rep->bt->n;

    }
  }
  FILE * fn = file;
  for (fila = 0; fila < trep->part; fila++) {
    for (columna = 0; columna < trep->part; columna++) {
      rep = trep->submatrices[fila][columna];
      if (rep == NULL )
        continue;
      load(rep->bn, fn);
      rep->bn_len = rep->bn->n;
    }
  }
  return trep;
}

void saveTreeRep(TREP * trep, FILE * file) {

	FILE * fv = file;


	fwrite(&(trep->part), sizeof(uint), 1, fv);
	fwrite(&(trep->tamSubm), sizeof(uint), 1, fv);

	fwrite(&(trep->numberOfNodes), sizeof(uint), 1, fv);
	fwrite(&(trep->numberOfEdges), sizeof(uint), 1, fv);

	fwrite(&(trep->repK1), sizeof(uint), 1, fv);
	fwrite(&(trep->repK2), sizeof(uint), 1, fv);
	fwrite(&(trep->maxRealLevel1), sizeof(uint), 1, fv);
	fwrite(&(trep->maxLevel1), sizeof(uint), 1, fv);
	fwrite(&(trep->maxLevel2), sizeof(uint), 1, fv);

	//  fwrite(&(trep->maxLevel),sizeof(uint),1,fv);
	//
	fwrite(&trep->zeroNode, sizeof(uint), 1, fv); //stores the number of words of the vocabulary
	fwrite(&trep->lenWords, sizeof(uint), 1, fv);
	//Writes the vocabulary to disk.
	int i;
	fwrite(trep->words, sizeof(char), trep->lenWords * trep->zeroNode, fv);

	FILE * fi = file;

	
	int fila, columna;
	MREP* rep;
	uint part = trep->part;

	for (fila = 0; fila < part; fila++) {
		for (columna = 0; columna < part; columna++) {
			rep = trep->submatrices[fila][columna];
			//  fclose(fi);
			//
			//  strcpy(filename,argv[3]);
			//  strcat(filename,".rs");
			//  FILE * fr = fopen(filename,"w");

			//fprintf(stderr,"fila: %d, columna: %d, edges: %d\n",fila,columna,rep->numberOfEdges);
			fwrite(&(rep->numberOfNodes), sizeof(uint), 1, fi);
			fwrite(&(rep->numberOfEdges), sizeof(uint), 1, fi);
			if (trep->submatrices[fila][columna]->numberOfEdges == 0)
				continue;
			fwrite(&(rep->cutBt), sizeof(uint), 1, fi);
			fwrite(&(rep->lastBt1_len), sizeof(uint), 1, fi);
			//fwrite(&(rep->maxLevel),sizeof(uint),1,fi);
			fwrite(&(rep->nleaves), sizeof(uint), 1, fi);
			//fwrite (rep->leavesInf,sizeof(uint),rep->nleaves*K2*K2/W+1,fi);
			saveFT(rep->compressIL, fi);
		}
	}


	FILE * ft = file;
	for (fila = 0; fila < part; fila++) {
		for (columna = 0; columna < part; columna++) {
			if (trep->submatrices[fila][columna]->numberOfEdges != 0)
				save(trep->submatrices[fila][columna]->bt, ft);
		}
	}

	FILE * fn = file;
	for (fila = 0; fila < part; fila++) {
		for (columna = 0; columna < part; columna++) {
			if (trep->submatrices[fila][columna]->numberOfEdges != 0)
				save(trep->submatrices[fila][columna]->bn, fn);
		}
	}


} 

/*
void saveTreeRep(TREP * trep, char * basename) {
	char *filename = (char *) mymalloc(sizeof(char) * (strlen(basename) + 5));
	strcpy(filename, basename);
	strcat(filename, ".tr");
	FILE * ft = fopen(filename, "w");
	uint part = trep->part;
	int fila, columna;
	MREP* rep;
	for (fila = 0; fila < part; fila++) {
		for (columna = 0; columna < part; columna++) {
			if (trep->submatrices[fila][columna]->numberOfEdges != 0)
				save(trep->submatrices[fila][columna]->bt, ft);
		}
	}
	fclose(ft);

	strcpy(filename, basename);
	strcat(filename, ".lv");
	FILE * fn = fopen(filename, "w");
	for (fila = 0; fila < part; fila++) {
		for (columna = 0; columna < part; columna++) {
			if (trep->submatrices[fila][columna]->numberOfEdges != 0)
				save(trep->submatrices[fila][columna]->bn, fn);
		}
	}
	fclose(fn);

	strcpy(filename, basename);
	strcat(filename, ".voc");
	FILE * fv = fopen(filename, "w");

	fwrite(&(trep->part), sizeof(uint), 1, fv);
	fwrite(&(trep->tamSubm), sizeof(uint), 1, fv);

	fwrite(&(trep->numberOfNodes), sizeof(uint), 1, fv);
	fwrite(&(trep->numberOfEdges), sizeof(uint), 1, fv);

	fwrite(&(trep->repK1), sizeof(uint), 1, fv);
	fwrite(&(trep->repK2), sizeof(uint), 1, fv);
	fwrite(&(trep->maxRealLevel1), sizeof(uint), 1, fv);
	fwrite(&(trep->maxLevel1), sizeof(uint), 1, fv);
	fwrite(&(trep->maxLevel2), sizeof(uint), 1, fv);

	//  fwrite(&(trep->maxLevel),sizeof(uint),1,fv);
	//
	fwrite(&trep->zeroNode, sizeof(uint), 1, fv); //stores the number of words of the vocabulary
	fwrite(&trep->lenWords, sizeof(uint), 1, fv);
	//Writes the vocabulary to disk.
	int i;
	for (i = 0; i < zeroNode; i++)
		fwrite(hash[positionInTH[i]].word, sizeof(char), trep->lenWords, fv);

	fclose(fv);

	strcpy(filename, basename);
	strcat(filename, ".cil");
	FILE * fi = fopen(filename, "w");

	for (fila = 0; fila < part; fila++) {
		for (columna = 0; columna < part; columna++) {
			rep = trep->submatrices[fila][columna];
			//  fclose(fi);
			//
			//  strcpy(filename,argv[3]);
			//  strcat(filename,".rs");
			//  FILE * fr = fopen(filename,"w");

			//fprintf(stderr,"fila: %d, columna: %d, edges: %d\n",fila,columna,rep->numberOfEdges);
			fwrite(&(rep->numberOfNodes), sizeof(uint), 1, fi);
			fwrite(&(rep->numberOfEdges), sizeof(uint), 1, fi);
			if (trep->submatrices[fila][columna]->numberOfEdges == 0)
				continue;
			fwrite(&(rep->cutBt), sizeof(uint), 1, fi);
			fwrite(&(rep->lastBt1_len), sizeof(uint), 1, fi);
			//fwrite(&(rep->maxLevel),sizeof(uint),1,fi);
			fwrite(&(rep->nleaves), sizeof(uint), 1, fi);
			//fwrite (rep->leavesInf,sizeof(uint),rep->nleaves*K2*K2/W+1,fi);
			saveFT(rep->compressIL, fi);
		}
	}
	fclose(fi);

	free(filename);

}

TREP * loadTreeRepresentation(char * basename) {
  TREP * trep=(TREP *) mymalloc(sizeof(TREP));
  MREP * rep;
  int i;
  trep = mymalloc(sizeof(struct treeRep));
  char *filename = (char *) mymalloc(sizeof(char) * (strlen(basename) + 5));

  strcpy(filename, basename);
  strcat(filename, ".voc");
  FILE * fv = fopen(filename, "r");

  if (1!=fread(&(trep->part), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->tamSubm), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->numberOfNodes), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->numberOfEdges), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->repK1), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->repK2), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->maxRealLevel1), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->maxLevel1), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&(trep->maxLevel2), sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&trep->zeroNode, sizeof(uint), 1, fv)) //reads the number of words of the vocabulary
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  if (1!=fread(&trep->lenWords, sizeof(uint), 1, fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
  trep->words = (unsigned char *) mymalloc(
      sizeof(unsigned char) * trep->lenWords * trep->zeroNode);
  if (trep->lenWords*trep->zeroNode!=fread(trep->words, sizeof(unsigned char), trep->lenWords * trep->zeroNode,
      fv))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}

  fclose(fv);
  trep->div_level_table1 = (uint *) mymalloc(sizeof(uint) * trep->maxLevel1);
  for (i = 0; i < trep->maxLevel1; i++)
    trep->div_level_table1[i] = exp_pow(K1, trep->maxRealLevel1 - i);

  trep->div_level_table2 = (uint *) mymalloc(sizeof(uint) * trep->maxLevel2);
  for (i = 0; i < trep->maxLevel2; i++)
    trep->div_level_table2[i] = exp_pow(K2, trep->maxLevel2 + L - i);
  trep->iniq = -1;
  trep->finq = -1;

  trep->submatrices = (MREP ***) mymalloc(sizeof(MREP **) * trep->part);

  int fila, columna;
  for (i = 0; i < trep->part; i++) {
    trep->submatrices[i] = (MREP **) mymalloc(sizeof(MREP *) * trep->part);
  }
        int numBytes=getMemoria();
        numBytes=getMemoria();

  strcpy(filename, basename);
  strcat(filename, ".cil");
  FILE * fi = fopen(filename, "r");

  int totalnumberLeaves = 0;
  
  for (fila = 0; fila < trep->part; fila++) {
    for (columna = 0; columna < trep->part; columna++) {
      int numberOfNodes,numberOfEdges;
      if (1!=fread(&(numberOfNodes), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}

      if (1!=fread(&(numberOfEdges), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}



      if (numberOfEdges == 0){
        trep->submatrices[fila][columna]=NULL;
        continue;
      }
      rep = (MREP *) mymalloc(sizeof(struct matrixRep));

      rep->bt = (bitRankW32Int *) mymalloc(
          sizeof(struct sbitRankW32Int));

      rep->bn = (bitRankW32Int *) mymalloc(sizeof(struct sbitRankW32Int));
      trep->submatrices[fila][columna] = rep;

      rep->numberOfNodes=numberOfNodes;
      rep->numberOfEdges=numberOfEdges;

      if (1!=fread(&(rep->cutBt), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
      if (1!=fread(&(rep->lastBt1_len), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}
      if (1!=fread(&(rep->nleaves), sizeof(uint), 1, fi))
	{fprintf(stderr,"loadTreeRep:error de lectura\n");return NULL;}

      rep->compressIL = loadFT(fi);
      totalnumberLeaves += rep->compressIL->listLength;
      rep->leavesInf = NULL;

    }
  }
  fclose(fi);
  strcpy(filename, basename);
  strcat(filename, ".tr");
  FILE * ft = fopen(filename, "r");
  for (fila = 0; fila < trep->part; fila++) {
    for (columna = 0; columna < trep->part; columna++) {
      rep = trep->submatrices[fila][columna];
      if (rep == NULL)
        continue;
      load(rep->bt, ft);
      rep->bt_len = rep->bt->n;

    }
  }
  fclose(ft);
  strcpy(filename, basename);
  strcat(filename, ".lv");
  FILE * fn = fopen(filename, "r");
  for (fila = 0; fila < trep->part; fila++) {
    for (columna = 0; columna < trep->part; columna++) {
      rep = trep->submatrices[fila][columna];
      if (rep == NULL )
        continue;
      load(rep->bn, fn);
      rep->bn_len = rep->bn->n;
    }
  }
  fclose(fn);
  free(filename);
  return trep;
}

*/

void destroyTreeRepresentation(TREP *trep) {
	int i, j;
	for (i = 0; i < trep->part; i++) {
		for (j = 0; j < trep->part; j++)
			destroyRepresentation(trep->submatrices[i][j]);
		free(trep->submatrices[i]);
	}
	free(trep->submatrices);
	free(trep->div_level_table1);
	free(trep->div_level_table2);

	//free(trep->info2[0]);
	//free(trep->info2[1]);
	//free(trep->info);
	//free(trep->element);
	//free(trep->basex);
	//free(trep->basey);

	free(trep->words);

	free(trep);
}

void compressInformationLeaves(TREP * trep) {
	//Recorrer todas las ils, calcular frecuencias y usar FT*
	MREP * rep;

	uint fila, columna;

	uint totalLeaves = 0;
	uint i, j;

	for (fila = 0; fila < trep->part; fila++) {
		for (columna = 0; columna < trep->part; columna++) {
			rep = trep->submatrices[fila][columna];
			if (trep->submatrices[fila][columna]->numberOfEdges != 0)
				totalLeaves += rep->nleaves;
		}
	}

	unsigned char * ilchar = (unsigned char *) mymalloc(
			sizeof(unsigned char) * totalLeaves * K2_3_char);

	unsigned char *aWord;
	unsigned int size;
	initialize(totalLeaves);
	//Creaci�n del vocabulario
	uint ilpos = 0;
	for (fila = 0; fila < trep->part; fila++) {
		for (columna = 0; columna < trep->part; columna++) {
			rep = trep->submatrices[fila][columna];
			if (trep->submatrices[fila][columna]->numberOfEdges == 0)
				continue;
			for (i = 0; i < rep->nleaves; i++) {
				aWord = &(ilchar[ilpos]); //the word parsed.

				for (j = 0; j < K2_3; j++) {
					if (bitget(rep->leavesInf,i*K2_3+j))
						bitsetchar(aWord,j);
					else
						bitcleanchar(aWord,j);

				}

				size = K2_3_char;
				j = search((unsigned char *) aWord, size, &addrInTH);

				if (j == zeroNode) {
					insertElement((unsigned char *) aWord, size, &addrInTH);
					hash[addrInTH].weight = 0;
					hash[addrInTH].size = 0;
					hash[addrInTH].len = K2_3_char;
					positionInTH[zeroNode] = addrInTH;
					zeroNode++;
				}

				hash[addrInTH].weight += 1;

				ilpos += K2_3_char;
			}
		}
	}

	//printf("Zeronode: %d\n",zeroNode);

	trep->zeroNode = zeroNode;
	trep->lenWords = K2_3_char;

	//Compresion de hojas

	int k = 0;
	//printf("Sorting the vocabulary by frequency\n");
	// Sorting the vocabulary by frequency.

	{ //Moves all the words with frequency = 1 to the end of the vocabulary.
		register int ii;
		register int kk;

		kk = zeroNode - 1;
		ii = 0;
		while (ii < kk) {
			while ((hash[positionInTH[ii]].weight != 1) && (ii < kk)) {
				ii++;
			}
			while ((hash[positionInTH[kk]].weight == 1) && (ii < kk)) {
				kk--;
			}

			if (ii < kk) {
				swap(&positionInTH[ii], &positionInTH[kk]);
				kk--;
				ii++;
			}
		}

		//k=ii;
		k = ii + 1; //the lenght of the vector to be sorted with qsort. So v[0 .. k-1]
	}

	//printf("Aplies qsort to the words with frequency > 1.\n");
	//Aplies qsort to the words with frequency > 1.
	qsort(positionInTH, k, sizeof(unsigned int), comparaFrecListaDesc);
	uint totalLeavesCount = 0;
	for (i = 0; i < zeroNode; i++) {
		hash[positionInTH[i]].codeword = i;
		totalLeavesCount += hash[positionInTH[i]].weight;
	}

	/********************** Beginning of the second pass **********************/
	//Compactando informaci�n de las hojas...

	ilpos = 0;
	for (fila = 0; fila < trep->part; fila++) {
		for (columna = 0; columna < trep->part; columna++) {
			rep = trep->submatrices[fila][columna];
			if (trep->submatrices[fila][columna]->numberOfEdges == 0)
				continue;
			uint * listIL = (uint *) mymalloc(sizeof(uint) * rep->nleaves);
			uint listILCount = 0;

			for (i = 0; i < rep->nleaves; i++) {

				aWord = &(ilchar[ilpos]); //the word parsed.
				size = K2_3_char;
				j = search((unsigned char *) aWord, size, &addrInTH);
				listIL[listILCount++] = hash[addrInTH].codeword;
				ilpos += K2_3_char;
			}

			//fprintf(stderr,"tamTotal: %d\n",tamTotal);


			rep->compressIL = createFT(listIL, rep->nleaves);

			free(listIL);
		}
	}
	trep->words = (unsigned char *) mymalloc(
			sizeof(unsigned char) * trep->zeroNode * trep->lenWords);

	int wc = 0;
	for (i = 0; i < zeroNode; i++) {
		for (j = 0; j < trep->lenWords; j++) {
			trep->words[wc++] = hash[positionInTH[i]].word[j];
		}
	}

	free(ilchar);

}

/*
    k-ary tree
        creation
        traversal  
        
    Notes: 
    1.  both creation and traversal follow BFS      
    2.  parent pointer is stored just to show that the tree is created
        correctly, its not used in any way to aid in creation and 
        traversal of the tree
*/

#include "kTree-construccion.h"

#define swap( x, y ) { int temp; temp=*(x); *(x)=*(y); *(y)=temp; }



typedef struct QUEUE
{
    NODE *element;
    struct QUEUE *link;
    int basex;
    int basey;
}QUEUE;/*for implementing BFS*/

typedef struct QUEUE2
{
    int element;
    struct QUEUE2 *link;
    int basey;
    int basex;
}QUEUE2;/*for implementing BFS*/

typedef struct QUEUECONS
{
    NODE *element;
    struct QUEUECONS *link;

}QUEUECONS;/*for implementing BFS*/

QUEUE * finalQUEUE;
QUEUE2 * finalQUEUE2;
QUEUECONS * finalQUEUECONS;



uint *div_level_table;

unsigned int * positionInTH;
unsigned int addrInTH;
unsigned int zeroNode;

int hasAnyBitSet(unsigned char * b){
	int i=0,bvalue=0;
	if(b!=NULL)
		for(i=0;i<K2_3_char;i++)
			bvalue = bvalue||(b[i]!=0);
	return bvalue;
}	


QUEUE * AddItem (QUEUE * listpointer, NODE * elem, int cantx, int canty) {

  /*QUEUE * lp = listpointer;
  if (listpointer != NULL) {
		while (listpointer -> link != NULL)
	    listpointer = listpointer -> link;
		listpointer -> link = (QUEUE  *) malloc (sizeof (struct QUEUE));
		listpointer = listpointer -> link;
		listpointer -> link = NULL;
		listpointer -> element = elem;
		listpointer -> basex = cantx;
		listpointer -> basey = canty;
		return lp;
	}
  else {
		listpointer = (QUEUE  *) malloc (sizeof (struct QUEUE));
		listpointer -> link = NULL;
		listpointer -> element = elem;
		listpointer -> basex = cantx;
		listpointer -> basey = canty;
		return listpointer;
  }
  */
    if(listpointer != NULL){
	  	QUEUE * lp = (QUEUE  *) malloc (sizeof (struct QUEUE));
	  	finalQUEUE -> link = lp;
			lp -> link = NULL;
			lp -> element = elem;
			lp -> basex = cantx;
			lp -> basey = canty;
			finalQUEUE = lp;
			return listpointer;
		}
		else{
			listpointer = (QUEUE  *) malloc (sizeof (struct QUEUE));
			listpointer -> link = NULL;
			listpointer -> element = elem;
			listpointer -> basex = cantx;
			listpointer -> basey = canty;
			finalQUEUE = listpointer;
			return listpointer;
		}
	
}

QUEUE * RemoveItem (QUEUE * listpointer) {

    QUEUE * tempp;
//    printf ("Element removed is %d\n", listpointer -> dataitem);
    tempp = listpointer -> link;
    free (listpointer);
    return tempp;
}

void ClearQueue (QUEUE * listpointer) {

    while (listpointer != NULL) {
				listpointer = (QUEUE *)RemoveItem (listpointer);
    }
}




void AddItem2 (TREP *trep, int elem, int cantx,int canty) {
  	if(trep->iniq!=-1){
	    trep->finq++;
			trep -> element[trep->finq] = elem;
			trep -> basex[trep->finq] = cantx;
			trep -> basey[trep->finq] = canty;
			
		}
		else{
			trep->iniq=0;
			trep->finq=0;
			trep -> element[trep->iniq] = elem;
			trep -> basex[trep->iniq] = cantx;
			trep -> basey[trep->iniq] = canty;
		}
}

void RemoveItem2 (TREP * trep) {

	trep->iniq++;
}

QUEUECONS * AddItemCONS (QUEUECONS * listpointer, NODE * elem) {

  	if(listpointer!=NULL){
	    QUEUECONS * lp = (QUEUECONS  *) malloc (sizeof (struct QUEUECONS));
	  	finalQUEUECONS -> link = lp;
			lp -> link = NULL;
			lp -> element = elem;
			finalQUEUECONS = lp;
			return listpointer;
		}
		else{
			listpointer = (QUEUECONS  *) malloc (sizeof (struct QUEUECONS));
			listpointer -> link = NULL;
			listpointer -> element = elem;
			finalQUEUECONS = listpointer;
			return listpointer;
		}
}

QUEUECONS * RemoveItemCONS (QUEUECONS * listpointer) {
    QUEUECONS * tempp;
//    printf ("Element removed is %d\n", listpointer -> dataitem);
    tempp = listpointer -> link;
    free (listpointer);
    return tempp;
}

uint exp_pow(uint base, uint pow){
	uint i, result = 1;
	for(i=0;i<pow;i++)
		result*=base;
//	fprintf(stderr,"En exp_pow: %d elevado a %d = %d\n",base,pow,result);
	return result;
}


/*------------------------------------------------------------------
 Function used by qsort to compare two elements a and b
------------------------------------------------------------------*/
int comparaFrecListaDesc(const void *a, const void *b)
{
	unsigned int *left,*right;
	left =  (unsigned int *) a;
	right = (unsigned int *) b;
    if ( hash[*left].weight < hash[*right].weight)
                return 1;
    if ( hash[*left].weight > hash[*right].weight)
                return -1;

    return 0;
}

/*------------------------------------------------------------------
 Initilizes the structures used.
 ---------------------------------------------------------------- */
void initialize (unsigned int sizeVoc){
	//unsigned long i;

	_memMgr = createMemoryManager();
    initialize_hash(sizeVoc);

  positionInTH = (unsigned int*) malloc (sizeVoc * sizeof(unsigned int));
	zeroNode = 0;
}


NODE * createKTree(int _K1, int _K2, int maxreallevel1, int maxlevel1, int maxlevel2){
	NODE * n = (NODE *) malloc(sizeof(struct node));
	n->child=NULL;
	n->data=0;
//	K1 = _K1;
//	K2 = _K2;
	max_real_level1 = maxreallevel1;
	max_Level1 = maxlevel1;
	max_Level2 = maxlevel2-L;
	numberNodes =0;
	numberLeaves = 0;
	numberTotalLeaves=0;
	return n;
}



void destroyRepresentation(MREP * rep){
	destroyBitRankW32Int(rep->bt);
	destroyBitRankW32Int(rep->bn);
	destroyFT(rep->compressIL);
	if(rep->leavesInf!=NULL)
		free(rep->leavesInf);
	free(rep);
}


uint * compactAdjacencyList(TREP * trep,MREP * rep, int x){
	int K1K1 = K1*K1;
	int K2K2 = K2*K2;
	
	if(rep->numberOfEdges==0)
		return trep->info;

	uint factorAdjust = K1K1/K2K2;
	trep->iniq=-1;
	trep->finq=-1;
	uint nleaf,posInf,realvalue,  nleafrelat;
//	uint summemoria =0;
	uint totalAdyNodes =0;
	int i, k, j, queuecont, conttmp,node,div_level,xrelat;
//	fprintf(stderr,"Aqui llego0\n");
	AddItem2(trep,0,0,x);
//	fprintf(stderr,"Tama�o de la estructura: %d\n",(int) sizeof(struct QUEUE2));
//	summemoria+=sizeof(struct QUEUE2);
//	fprintf(stderr,"Aqui llego1\n");
	queuecont = 1;
	for(i=0;i<trep->maxLevel1-1;i++){
		conttmp = 0;
	//		fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table1[i];
		//fprintf(stderr,"div_level=%d\n",div_level);
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K1, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
				for(j=0;j<K1;j++){
					xrelat = trep->basey[trep->iniq];
					node = xrelat/div_level*K1 + j;
					node += trep->element[trep->iniq];
					//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);
		
					if(isBitSet(rep->bt,node)){
						//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
						conttmp++;
						//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]+xrelat/div_level*div_level);
						//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rank(rep->bt,node)*K1*K1);
						AddItem2(trep,rank(rep->bt,node)*K1K1,trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]%div_level);
	//						summemoria+=sizeof(struct QUEUE2);
					}
				}
			
			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

//fprintf(stderr,"cutBt: %d, factorAdjust: %d\n",rep->cutBt, factorAdjust);
	for(i=trep->maxLevel1-1;i<trep->maxLevel1;i++){
		conttmp = 0;
	//		fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table1[i];
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K1, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
				for(j=0;j<K1;j++){
					xrelat = trep->basey[trep->iniq];
					node = xrelat/div_level*K1 + j;
					node += trep->element[trep->iniq];
					//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);
		
					if(isBitSet(rep->bt,node)){
						//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
						conttmp++;
						//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]+xrelat/div_level*div_level);
						//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rep->cutBt + (rank(rep->bt,node)*K1*K1-rep->cutBt)/factorAdjust);
						AddItem2(trep,rep->cutBt + (rank(rep->bt,node)*K1K1-rep->cutBt)/factorAdjust,trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]%div_level);
	//						summemoria+=sizeof(struct QUEUE2);
					}
				}
			
			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	uint cutPreRank = rank(rep->bt,rep->lastBt1_len-1);
	//fprintf(stderr,"cutPreRank %d lastBt_1: %d\n",cutPreRank,rep->lastBt1_len);
		for(i=0;i<trep->maxLevel2-1;i++){
		conttmp = 0;
		//	fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table2[i];
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K2, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
				for(j=0;j<K2;j++){
					xrelat = trep->basey[trep->iniq];
					node = xrelat/div_level*K2 + j;
					node += trep->element[trep->iniq];
					//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);
		
					if(isBitSet(rep->bt,node)){
						//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
						conttmp++;
						//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basex[trep->iniq]+xrelat/div_level*div_level);
						//fprintf(stderr,"cutBt: %d, rank: %d, cutPrerank: %d\n",rep->cutBt ,rank(rep->bt,node),cutPreRank);
						//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rep->cutBt + (rank(rep->bt,node)-cutPreRank)*K2*K2);
						AddItem2(trep,rep->cutBt + (rank(rep->bt,node-1)-cutPreRank)*K2K2,trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]%div_level);
	//						summemoria+=sizeof(struct QUEUE2);
					}
				}
			
			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	
	while(trep->iniq<=trep->finq){
		nleaf = trep->element[trep->iniq]-rep->bt_len;
		//fprintf(stderr,"\nMirando la hoja %d con basey %d\n",nleaf,trep->basey[trep->iniq]);
		for(j=0;j<K2;j++){
			nleafrelat = nleaf + (trep->basey[trep->iniq]/K2_2)*K2+j;
			//fprintf(stderr,"Relativo %d:, isbitset: %d\n",nleafrelat,isBitSet(rep->bn,nleafrelat));
			if(isBitSet(rep->bn,nleafrelat)){
				//fprintf(stderr,"Esta el bit a 1! posinf sera: %d\n",(rank(rep->bn,nleafrelat)-1)*K2*K2);
				//posInf = (rank(rep->bn,nleafrelat)-1)*K2K2;
				posInf = rank(rep->bn,nleafrelat);
					
				realvalue = accessFT(rep->compressIL,posInf);				
				for(i=0;i<K2_2;i++){
//					fprintf(stderr,"probando relativo %d, posinf: %d ....contenido: %lld Acierto: bitget %d bitgetcharsobre: %x %x\n",i+(x%K2_2)*K2_2, posInf,trep->words[realvalue*trep->lenWords],bitgetchar(&(trep->words[realvalue*trep->lenWords]),(i+(x%K2_2)*K2_2)),(trep->words[realvalue*trep->lenWords]),(trep->words[realvalue*trep->lenWords+4]));
					if(bitgetchar(&(trep->words[realvalue*trep->lenWords]),(i+(x%K2_2)*K2_2))){
					//if(bitget(rep->leavesInf,posInf+(i+(x%K2)*K2))){
						//fprintf(stderr,"%d\n",trep->basex[trep->iniq]+i+K2*j);
						//totalAdyNodes++;
						trep->info[0]++;
						trep->info[trep->info[0]]=trep->basex[trep->iniq]+i+K2_2*j+trep->columna*trep->tamSubm;
					}
				}
			}
		}
		RemoveItem2(trep);
	}
	//fprintf(stderr,"suma de memoria total....%d\n",summemoria);
	//trep->info[0]=totalAdyNodes;
	return trep->info;
}


uint * compactInverseList(TREP * trep,MREP * rep, int y){
	if(rep->numberOfEdges==0)
		return trep->info;

	int K1K1 = K1*K1;
	int K2K2 = K2*K2;
	uint factorAdjust = K1K1/K2K2;
	trep->iniq=-1;
	trep->finq=-1;
	uint nleaf,posInf,realvalue,  nleafrelat;
//	uint summemoria =0;
	uint totalAdyNodes =0;
	int i, k, j, queuecont, conttmp,node,div_level,yrelat;
//	fprintf(stderr,"Aqui llego0\n");
	AddItem2(trep,0,y,0);
//	fprintf(stderr,"Tama�o de la estructura: %d\n",(int) sizeof(struct QUEUE2));
//	summemoria+=sizeof(struct QUEUE2);
//	fprintf(stderr,"Aqui llego1\n");
	queuecont = 1;
	for(i=0;i<trep->maxLevel1-1;i++){
		conttmp = 0;
	//		fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table1[i];
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K1, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
				for(j=0;j<K1;j++){
					yrelat = trep->basex[trep->iniq];
					node = K1*j + yrelat/div_level;
					node += trep->element[trep->iniq];
					//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);
		
					if(isBitSet(rep->bt,node)){
						//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
						conttmp++;
						//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]+xrelat/div_level*div_level);
						//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rank(rep->bt,node)*K1*K1);
						AddItem2(trep,rank(rep->bt,node)*K1K1,trep->basex[trep->iniq]%div_level,trep->basey[trep->iniq]+j*div_level);
	//						summemoria+=sizeof(struct QUEUE2);
					}
				}
			
			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

//fprintf(stderr,"cutBt: %d, factorAdjust: %d\n",rep->cutBt, factorAdjust);
	for(i=trep->maxLevel1-1;i<trep->maxLevel1;i++){
		conttmp = 0;
	//		fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table1[i];
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K1, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
				for(j=0;j<K1;j++){
					yrelat = trep->basex[trep->iniq];
					node = K1*j+ yrelat/div_level;
					node += trep->element[trep->iniq];
					//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);
		
					if(isBitSet(rep->bt,node)){
						//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
						conttmp++;
						//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basey[trep->iniq]+xrelat/div_level*div_level);
						//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rep->cutBt + (rank(rep->bt,node)*K1*K1-rep->cutBt)/factorAdjust);
						AddItem2(trep,rep->cutBt + (rank(rep->bt,node)*K1K1-rep->cutBt)/factorAdjust,trep->basex[trep->iniq]%div_level,trep->basey[trep->iniq]+j*div_level);
	//						summemoria+=sizeof(struct QUEUE2);
					}
				}
			
			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	uint cutPreRank = rank(rep->bt,rep->lastBt1_len-1);
	//fprintf(stderr,"cutPreRank %d lastBt_1: %d\n",cutPreRank,rep->lastBt1_len);
		for(i=0;i<trep->maxLevel2-1;i++){
		conttmp = 0;
		//	fprintf(stderr,"Aqui llego2\n");

		div_level = trep->div_level_table2[i];
		//div_level_x = x/div_level*K1;
		//fprintf(stderr,"Nivel: %d de K2, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
				for(j=0;j<K2;j++){
					yrelat = trep->basex[trep->iniq];
					node = K2*j + yrelat/div_level;
					node += trep->element[trep->iniq];
					//fprintf(stderr,"node: %d %d\n",node,trep->basey[trep->iniq]);
		
					if(isBitSet(rep->bt,node)){
						//fprintf(stderr,"Isbitsetnode: %d %d\n",node,xrelat);
						conttmp++;
						//fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",trep->basex[trep->iniq]+j*div_level,trep->basex[trep->iniq]+xrelat/div_level*div_level);
						//fprintf(stderr,"cutBt: %d, rank: %d, cutPrerank: %d\n",rep->cutBt ,rank(rep->bt,node),cutPreRank);
						//fprintf(stderr,"Y con element: rank(%d)=%d\n",node,rep->cutBt + (rank(rep->bt,node)-cutPreRank)*K2*K2);
						AddItem2(trep,rep->cutBt + (rank(rep->bt,node-1)-cutPreRank)*K2K2,trep->basex[trep->iniq]%div_level,trep->basey[trep->iniq]+j*div_level);
	//						summemoria+=sizeof(struct QUEUE2);
					}
				}
			
			RemoveItem2(trep);
		}
		queuecont = conttmp;
	}

	
	while(trep->iniq<=trep->finq){
		nleaf = trep->element[trep->iniq]-rep->bt_len;
		//fprintf(stderr,"\nMirando la hoja %d con basey %d\n",nleaf,trep->basey[trep->iniq]);
		for(j=0;j<K2;j++){
			nleafrelat = nleaf + (trep->basex[trep->iniq]/K2_2)+K2*j;
			//fprintf(stderr,"Relativo %d:, isbitset: %d\n",nleafrelat,isBitSet(rep->bn,nleafrelat));
			if(isBitSet(rep->bn,nleafrelat)){
				//fprintf(stderr,"Esta el bit a 1! posinf sera: %d\n",(rank(rep->bn,nleafrelat)-1)*K2*K2);
				//posInf = (rank(rep->bn,nleafrelat)-1)*K2K2;

				posInf = rank(rep->bn,nleafrelat);
				realvalue = accessFT(rep->compressIL,posInf);

				for(i=0;i<K2_2;i++){
					//fprintf(stderr,"probando relativo %d, total %d....Acierto: bitget%d\n",i+(x%K2)*K2, posInf+(i+(x%K2)*K2),bitget(rep->leavesInf,posInf+(i+(x%K2)*K2)));
					if(bitgetchar(&(trep->words[realvalue*trep->lenWords]),(i*K2_2+(y%K2_2)))){
					//if(bitget(rep->leavesInf,posInf+(i*K2+(y%K2)))){
						//fprintf(stderr,"%d\n",trep->basex[trep->iniq]+i+K2*j);
						trep->info[0]++;
						trep->info[trep->info[0]]=trep->basey[trep->iniq]+i+K2_2*j+trep->fila*trep->tamSubm;
					}
				}
			}
		}
		RemoveItem2(trep);
	}
	//fprintf(stderr,"suma de memoria total....%d\n",summemoria);
	//trep->info[0]=totalAdyNodes;
	return trep->info;
}


MREP * createRepresentation(NODE * root, uint numberOfNodes,uint numberOfEdges){
	MREP * rep;
	rep = malloc(sizeof(struct matrixRep));
	//trep->maxRealLevel1 = max_real_level1;
	//trep->maxLevel1 = max_Level1;
	//trep->maxLevel2 = max_Level2;
	//rep-> K1 = K1;
	//rep-> K2 = K2;
	rep->numberOfNodes = numberOfNodes;
	rep->numberOfEdges = numberOfEdges;
	uint bits_BT_len = numberNodes;
	uint bits_BN_len = numberTotalLeaves;
	uint bits_LI_len = numberLeaves*K2_2*K2_2;
	bitRankW32Int *BT, *BN;
	uint * bits_BT = (uint*)malloc(sizeof(uint)*((bits_BT_len+W-1)/W));
	uint * bits_BN = (uint*)malloc(sizeof(uint)*((bits_BN_len+W-1)/W));
	uint * bits_LI = (uint*)malloc(sizeof(uint)*((bits_LI_len+W-1)/W));
	int i, k, j, queuecont, conttmp,node,div_level, pos=0;
	//fprintf(stderr,"numberNodes = %d\n.... long entero: %d",numberNodes,(bits_BT_len+W-1)/W);
	for(i=0; i<(W-1+bits_BT_len)/W;i++)
		bits_BT[i]=0;
	for(i=0; i<(W-1+bits_BN_len)/W;i++)
		bits_BN[i]=0;
	for(i=0; i<(W-1+bits_LI_len)/W;i++)
		bits_LI[i]=0;
		
	char isroot=1;
	QUEUECONS * q=NULL;
	finalQUEUECONS = q;
	q = AddItemCONS(q,root);
	queuecont = 1;
	for(i=0;i<max_Level1;i++){
		conttmp = 0;
		div_level = exp_pow(K1,max_real_level1-i);
		//div_level_x = x/div_level*K1;
//		printf("Nivel: %d, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
		//	fprintf(stderr,"Nivel: %d, div_level %d, queuecont %d k %d\n",i,div_level,queuecont, k);

			if(q->element->child!=NULL){
				for(j=0;j<K1*K1;j++){
					node = j;
					//fprintf(stderr,"node: %d pos %d\n",node,pos);
					conttmp++;
				//	fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",q->basex+j*div_level,q->basey+xrelat/div_level*div_level);
					q=AddItemCONS(q,q->element->child[node]);
					
					}
					if(!isroot)
						bitset(bits_BT,pos);

			}
			if(!isroot)
				pos++;
			isroot=0;
			//fprintf(stderr,"Contenido bitmap:  ");
			//for(j=0; j<pos;j++)
			//	fprintf(stderr,"%x",bitget(bits_BT,j));
			//fprintf(stderr,"\n");
			free(q->element);
			q = (QUEUECONS *)RemoveItemCONS(q);
		}
		queuecont = conttmp;
	}
	//fprintf(stderr,"pos para cutBt: %d lastBt1_len %d\n",pos+queuecont,pos);
	rep->lastBt1_len = pos;
	rep->cutBt = pos+queuecont;
	for(i=0;i<max_Level2;i++){
		conttmp = 0;
		div_level = exp_pow(K2,max_Level2+L-i);
		//div_level_x = x/div_level*K1;
	//	printf("Nivel: %d, div_level %d, queuecont %d\n",i,div_level,queuecont);
		for(k=0;k<queuecont;k++){
		//	fprintf(stderr,"Nivel: %d, div_level %d, queuecont %d k %d\n",i,div_level,queuecont, k);

			if(q->element->child!=NULL){
				for(j=0;j<K2*K2;j++){
					node = j;
			//		fprintf(stderr,"node: %d\n",node);
					conttmp++;
				//	fprintf(stderr,"Anhadiendo nodo hijo con basex %d basey %d\n",q->basex+j*div_level,q->basey+xrelat/div_level*div_level);
					q=AddItemCONS(q,q->element->child[node]);
					
					}
					if(!isroot)
						bitset(bits_BT,pos);
					free(q->element->child);

			}
				pos++;
			//fprintf(stderr,"Contenido bitmap:  ");
			//for(j=0; j<pos;j++)
			//	fprintf(stderr,"%x",bitget(bits_BT,j));
			//fprintf(stderr,"\n");
			
			q = (QUEUECONS *)RemoveItemCONS(q);
		}
		queuecont = conttmp;
	}

	BT = createBitRankW32Int(bits_BT, bits_BT_len , 1, 20); 


	rep->bt = BT;
	rep->bt_len = pos;

	pos=0;
	uint pos_inf=0;
//	fprintf(stderr,"Empezando bitmap de hojas utiles\n");
	while(q!=NULL){
//		fprintf(stderr,"quecont: %d\n Datos: %x\n",queuecont,q->element->data);
		if(hasAnyBitSet((q->element)->data)){
//			fprintf(stderr,"poniendo a uno el bit %d\n",pos);
				bitset(bits_BN,pos);
			
				for(i=0;i<K2_2*K2_2;i++){
			//fprintf(stderr,"probando %d, %x %x %x\n",i+(x%K1)*K1,(q->element)->data,(0x1<<(i+(x%K1)*K1)),((q->element)->data)&(0x1<<(i+(x%K1)*K1)));
					if(bitgetchar((q->element)->data,i)){
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
		q = (QUEUECONS *)RemoveItemCONS(q);
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

	BN = createBitRankW32Int(bits_BN, bits_BN_len , 1, 20); 

	rep->bn = BN;
	rep->bn_len = pos;
	
	rep->leavesInf = bits_LI;
	rep->nleaves = numberLeaves;
	
	//printf("Representacion:\n%d bits para arbol\n%d bits para hojas\n%d bits hojas utiles\n", rep->bt_len, rep->bn_len, rep->nleaves*K2*K2); 
	return rep;
}

void insertNode(NODE * root, int x, int y){
	//fprintf(stderr,"MaxLevel1: %d MaxLevel2 %d K1: %d K2 %d\n",max_Level1,max_Level2,K1,K2);
	uint i,node, div_level;
	int l=0;
	NODE * n = root;
	//fprintf(stderr,"\nEntrando x: %d y: %d\n",x,y);
	while(l<max_Level1){
		//printf("Level %d de K1, x: %d y %d\n",l,x,y);
		div_level = exp_pow(K1,max_real_level1-l);
		//fprintf(stderr,"x/div_level = %d, y/div_level = %d, %d, %d\n",(x / div_level),y /div_level,(x / div_level)*K1,(x / div_level)*K1+y /div_level);
		node = (x / div_level)*K1+y /div_level;
			if(n->child==NULL){
				//fprintf(stderr,"No existian hijos, creando los hijos");
				//if(l<max_Level1-1)
					numberNodes+=K1*K1;
				//else
				//	numberTotalLeaves+=K1*K1;
				//fprintf(stderr,"number nodes: %d\n",numberNodes);
				n->child = (NODE **)malloc(sizeof(NODE *)*K1*K1);
				for(i=0;i<K1*K1;i++){
					n->child[i]=(NODE *) malloc(sizeof(struct node));
					n->child[i]->child=NULL;
					n->child[i]->data=0;
				}
			}
			//fprintf(stderr,"Yendo al hijo %d\n",node);
			n = n->child[node];
			
		
		
		//fprintf(stderr,"bucle: x: %d, y: %d... node %d\n",x,y,node);
		x = x % div_level;
		y = y % div_level;
		l++;
	}
	
	l=0;
	int j;
		while(l<=max_Level2){
	//fprintf(stderr,"MaxLevel1: %d MaxLevel2 %d K1: %d K2 %d\n",max_Level1,max_Level2,K1,K2);
		//fprintf(stderr,"Level %d de K2, x: %d y %d\n",l,x,y);
		div_level = exp_pow(K2,max_Level2+L-l);
		//fprintf(stderr,"x/div_level = %d, y/div_level = %d, %d, %d\n",(x / div_level),y /div_level,(x / div_level)*K2,(x / div_level)*K2+y /div_level);
		node = (x / div_level)*K2+y /div_level;
		if(l==max_Level2){
			//fprintf(stderr,"Dentro de la hoja node: %d\n",node);
			if(!hasAnyBitSet(n->data)){
				numberLeaves++;
				/*
			//	fprintf(stderr,"Creando la hoja\n");
				n->data=(char *)malloc(sizeof(char)*K1*K1);
				for(i=0;i<K1*K1;i++)
					n->data[i]=0;*/
			}
			node = x *K2_2+y ;
			bitsetchar(n->data,node);
			//fprintf(stderr,"Introduciendo un 1 en la hoja %d: %x\n",node,n->data);
			//n->data[node]=1;
		}
		else{
			if(n->child==NULL){
				//fprintf(stderr,"No existian hijos, creando los hijos");
				if(l<max_Level2-1)
					numberNodes+=K2*K2;
				else
					numberTotalLeaves+=K2*K2;
				//fprintf(stderr,"number nodes: %d\n",numberNodes);
				n->child = (NODE **)malloc(sizeof(NODE *)*K2*K2);
				for(i=0;i<K2*K2;i++){
					n->child[i]=(NODE *) malloc(sizeof(struct node));
					n->child[i]->child=NULL;
					
					if(l==max_Level2-1){
						n->child[i]->data=malloc(sizeof(char)*K2_3_char);
						//fprintf(stderr,"haciendo un malloc de %d bytes, pos: %x\n",K_2/8,n->child[i]->data);
						for(j=0;j<K2_3_char;j++)
							n->child[i]->data[j]=0;
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




TREP * createTreeRep(uint nodesOrig,uint edges,uint part,uint subm, uint max_real_level1, uint max_level1, uint max_level2, uint _K1, uint _K2){
		TREP * trep;
		trep = malloc(sizeof(struct treeRep));
		trep->part=part;
    trep->tamSubm=subm;
    trep->numberOfNodes=nodesOrig;
    trep->numberOfEdges=edges;
    trep->maxRealLevel1=max_real_level1;//-L;
   	trep->maxLevel1=max_level1;
   	trep->maxLevel2=max_level2-L;
   	trep->repK1=_K1;
   	trep->repK2=_K2;
    
    trep->submatrices=(MREP ***)malloc(sizeof(MREP **)*part);
    int i,j;
    for(i=0;i<part;i++){
    	trep->submatrices[i]=(MREP **)malloc(sizeof(MREP *)*part);
    }


		
		trep->info = (uint *)malloc(sizeof(uint)*MAX_INFO);
			
		trep->info2[0] = (uint *)malloc(sizeof(uint)*MAX_INFO);
	
		trep->info2[1] = (uint *)malloc(sizeof(uint)*MAX_INFO);
	
		trep->element = (uint *)malloc(sizeof(uint)*MAX_INFO);	
	
		trep->basex = (uint *)malloc(sizeof(uint)*MAX_INFO);
	
		trep->basey = (uint *)malloc(sizeof(uint)*MAX_INFO);
	
		trep->iniq = -1;
		trep->finq =-1;
	trep->div_level_table1 = (uint *)malloc(sizeof(uint)*trep->maxLevel1);
	for(i=0;i<trep->maxLevel1;i++){
		trep->div_level_table1[i]=exp_pow(K1,trep->maxRealLevel1-i);
	}
	
	trep->div_level_table2 = (uint *)malloc(sizeof(uint)*trep->maxLevel2);
	for(i=0;i<trep->maxLevel2;i++){
		trep->div_level_table2[i]=exp_pow(K2,trep->maxLevel2+L-i);
	}

    return trep;

}

void insertIntoTreeRep(TREP * trep, MREP * rep, uint i, uint j){
	trep->submatrices[i][j]=rep;
	
}

/*
void saveTreeRep(TREP * trep, char * basename){
  char *filename = (char *) malloc(sizeof(char)*(strlen(basename)+5));
  strcpy(filename,basename);
  strcat(filename,".tr");
  FILE * ft = fopen(filename,"w");
  uint part= trep->part;
	int fila,columna;
	MREP* rep;
	for(fila=0;fila<part;fila++){
		for(columna=0;columna<part;columna++){
			if(trep->submatrices[fila][columna]->numberOfEdges!=0)
				save(trep->submatrices[fila][columna]->bt,ft);
		}
	}
  fclose(ft);

  
  strcpy(filename,basename);
  strcat(filename,".lv");
  FILE * fn = fopen(filename,"w");
	for(fila=0;fila<part;fila++){
		for(columna=0;columna<part;columna++){
				if(trep->submatrices[fila][columna]->numberOfEdges!=0)
				  save(trep->submatrices[fila][columna]->bn,fn);
			}
		}
  fclose(fn);  

  
  strcpy(filename,basename);
  strcat(filename,".voc");
  FILE * fv = fopen(filename,"w");

  fwrite(&(trep->part),sizeof(uint),1,fv);
  fwrite(&(trep->tamSubm),sizeof(uint),1,fv);

  fwrite(&(trep->numberOfNodes),sizeof(uint),1,fv);
  fwrite(&(trep->numberOfEdges),sizeof(uint),1,fv);
  
  fwrite(&(trep->repK1),sizeof(uint),1,fv);
  fwrite(&(trep->repK2),sizeof(uint),1,fv);
  fwrite(&(trep->maxRealLevel1),sizeof(uint),1,fv);
  fwrite(&(trep->maxLevel1),sizeof(uint),1,fv);
  fwrite(&(trep->maxLevel2),sizeof(uint),1,fv);

//  fwrite(&(trep->maxLevel),sizeof(uint),1,fv);
//  
  fwrite(&trep->zeroNode,sizeof(uint),1,fv); //stores the number of words of the vocabulary
	fwrite(&trep->lenWords,sizeof(uint),1,fv);
	//Writes the vocabulary to disk.
	int i;
	fwrite(trep->words, sizeof(char), trep->lenWords * trep->zeroNode, fv);
//	for (i=0;i<zeroNode;i++)
//		fwrite(hash[positionInTH[i]].word,sizeof(char),trep->lenWords,fv);

  fclose(fv);

  strcpy(filename,basename);
  strcat(filename,".cil");
  FILE * fi = fopen(filename,"w");

 
	for(fila=0;fila<part;fila++){
		for(columna=0;columna<part;columna++){  
		  rep=trep->submatrices[fila][columna];
		//  fclose(fi);
		//  
		//  strcpy(filename,argv[3]);
		//  strcat(filename,".rs");
		//  FILE * fr = fopen(filename,"w");
		  
		  //fprintf(stderr,"fila: %d, columna: %d, edges: %d\n",fila,columna,rep->numberOfEdges);
		  fwrite(&(rep->numberOfNodes),sizeof(uint),1,fi);
		  fwrite(&(rep->numberOfEdges),sizeof(uint),1,fi);
 			if(trep->submatrices[fila][columna]->numberOfEdges==0)
				continue;
		  fwrite(&(rep->cutBt),sizeof(uint),1,fi);
			fwrite(&(rep->lastBt1_len),sizeof(uint),1,fi);
		  //fwrite(&(rep->maxLevel),sizeof(uint),1,fi);
		  fwrite(&(rep->nleaves),sizeof(uint),1,fi);
		  //fwrite (rep->leavesInf,sizeof(uint),rep->nleaves*K2*K2/W+1,fi);
		  saveFT(rep->compressIL,fi);
		}
	}
  fclose(fi);   
  
  
 
  
  
	free(filename);
	
	
}



TREP * loadTreeRepresentation(char * basename){
	TREP * trep;
	MREP * rep;
	int i,j,k;
	
		trep = malloc(sizeof(struct treeRep));
  char *filename = (char *) malloc(sizeof(char)*(strlen(basename)+5));

//    trep->maxLevel=maxlevel;
//    trep->div_level_table;
    
//    ttrep->info;
//    ttrep->info2[2];
//    
//    ttrep->element;
//    ttrep->basex;
//    ttrep->basey;
//    ttrep->iniq;
//    ttrep->finq;
    
	strcpy(filename,basename);
  strcat(filename,".voc");
  FILE * fv = fopen(filename,"r");

  fread(&(trep->part),sizeof(uint),1,fv);
  fread(&(trep->tamSubm),sizeof(uint),1,fv);

  fread(&(trep->numberOfNodes),sizeof(uint),1,fv);
  fread(&(trep->numberOfEdges),sizeof(uint),1,fv);
	
	
	fread(&(trep->repK1),sizeof(uint),1,fv);
  //fprintf(stderr,"K1 : %d\n",trep->repK1);
  fread(&(trep->repK2),sizeof(uint),1,fv);
  //fprintf(stderr,"K2 : %d\n",trep->repK2);
  //K2 = trep->K2;
  //K1 = trep->K1;
  fread(&(trep->maxRealLevel1),sizeof(uint),1,fv);
  //fprintf(stderr,"Max Real Level1 : %d\n",trep->maxRealLevel1);
  fread(&(trep->maxLevel1),sizeof(uint),1,fv);
  //fprintf(stderr,"Max Level1 : %d\n",trep->maxLevel1);
  fread(&(trep->maxLevel2),sizeof(uint),1,fv);
  //fprintf(stderr,"Max Level2 : %d\n",trep->maxLevel2);

	
//	fread(&(trep->maxLevel),sizeof(uint),1,fv);
	
	fread(&trep->zeroNode,sizeof(uint),1,fv); //reads the number of words of the vocabulary
	fread(&trep->lenWords,sizeof(uint),1,fv);
	//fprintf(stderr,"zeroN:%d, len:%d\n",trep->zeroNode,trep->lenWords);
	trep->words = (unsigned char *) malloc(sizeof(char)*trep->lenWords*trep->zeroNode);
	fread(trep->words,sizeof(unsigned char),trep->lenWords*trep->zeroNode,fv);

	
	fclose(fv);
	
	
	trep->div_level_table1 = (uint *)malloc(sizeof(uint)*trep->maxLevel1);
	for(i=0;i<trep->maxLevel1;i++)
		trep->div_level_table1[i]=exp_pow(K1,trep->maxRealLevel1-i);
	
	trep->div_level_table2 = (uint *)malloc(sizeof(uint)*trep->maxLevel2);
	for(i=0;i<trep->maxLevel2;i++)
		trep->div_level_table2[i]=exp_pow(K2,trep->maxLevel2+L-i);

	
	//fprintf(stderr,"part: %d subm: %d nodes: %u edges: %d\n",trep->part,trep->tamSubm,trep->numberOfNodes,trep->numberOfEdges);



	trep->info = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->info2[0] = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->info2[1] = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->element = (uint *)malloc(sizeof(uint)*MAX_INFO);	
	trep->basex = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->basey = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->iniq = -1;
	trep->finq =-1;

    trep->submatrices=(MREP ***)malloc(sizeof(MREP **)*trep->part);
    
    int fila,columna;
    for(i=0;i<trep->part;i++){
    	trep->submatrices[i]=(MREP **)malloc(sizeof(MREP *)*trep->part);
    }

	for(i=0;i<trep->part;i++){
		for(j=0;j<trep->part;j++){
			rep = (MREP *) malloc(sizeof(struct matrixRep));
			rep->bt = (bitRankW32Int *) malloc(sizeof(struct sbitRankW32Int));
			rep->bn = (bitRankW32Int *) malloc(sizeof(struct sbitRankW32Int));				
			trep->submatrices[i][j]=rep;
				}
	}
   
 	strcpy(filename,basename);
  strcat(filename,".cil");
  FILE * fi = fopen(filename,"r");
 
	int totalnumberLeaves=0;

	for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){  
 			//fprintf(stderr,"IL: fila= %d, columna=%d, aristas desde %d y desde %d, (%2.2f\%)\n",fila,columna,fila*trep->tamSubm, columna*trep->tamSubm,(float)(fila*trep->part+columna)*100/trep->part/trep->part);
	
		  rep=trep->submatrices[fila][columna];
  		fread(&(rep->numberOfNodes),sizeof(uint),1,fi);
	//	  fprintf(stderr,"Number of nodes : %d\n",rep->numberOfNodes);
		  fread(&(rep->numberOfEdges),sizeof(uint),1,fi);
		  //fprintf(stderr,"Number of edges : %d\n",rep->numberOfEdges);
			if(rep->numberOfEdges==0)
				continue;

      fread(&(rep->cutBt),sizeof(uint),1,fi);

	    fread(&(rep->lastBt1_len),sizeof(uint),1,fi);

	
	//	  fprintf(stderr,"Max Level : %d\n",rep->maxLevel);
		  fread(&(rep->nleaves),sizeof(uint),1,fi);
	//	  fprintf(stderr,"Leaves to read: %d\n",rep->nleaves);
		
	//	  fprintf(stderr,"Leaves to read: %d\n",rep->nleaves);
		//rep->leavesInf = (uint *)malloc(sizeof(uint)*(rep->nleaves*K2*K2/W+1));
		//fread(rep->leavesInf,sizeof(uint),rep->nleaves*K2*K2/W+1,fi);
		rep->compressIL = loadFT(fi);
		totalnumberLeaves+=rep->compressIL->listLength;  
		rep->leavesInf=NULL;
		  
		  
		}
	}
  fclose(fi);   
  
  //fprintf(stderr,"totalnumberLeaves: %d\n",totalnumberLeaves);
	
	//fprintf(stderr,"Recovering tree bitmap\n");
	strcpy(filename,basename);
  strcat(filename,".tr");
  FILE * ft = fopen(filename,"r");
	for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){  
 			//fprintf(stderr,"TR: fila= %d, columna=%d, aristas desde %d y desde %d, (%2.2f\%)\n",fila,columna,fila*trep->tamSubm, columna*trep->tamSubm,(float)(fila*trep->part+columna)*100/trep->part/trep->part);
	
		  rep=trep->submatrices[fila][columna];
			if(rep->numberOfEdges==0)
				continue;
  		load(rep->bt,ft);
  		rep->bt_len = rep->bt->n;
  
		}
	}
  fclose(ft);
  //fprintf(stderr,"Bitmap recovered (len: %d bits)\n",rep->bt_len);
    
  //fprintf(stderr,"Recovering leaves bitmap\n");
  strcpy(filename,basename);
  strcat(filename,".lv");
  FILE * fn = fopen(filename,"r");
  for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){  
 			//fprintf(stderr,"LV: fila= %d, columna=%d, aristas desde %d y desde %d, (%2.2f\%)\n",fila,columna,fila*trep->tamSubm, columna*trep->tamSubm,(float)(fila*trep->part+columna)*100/trep->part/trep->part);
	
		  rep=trep->submatrices[fila][columna];
		  
		  if(rep->numberOfEdges==0)
				continue;
		  load(rep->bn,fn);
		  rep->bn_len = rep->bn->n;
		}
	}
  fclose(fn);  
  
  

  
//  fprintf(stderr,"Bitmap recovered (len: %d bits)\n",rep->bn_len);
//    
//  fprintf(stderr,"Recovering leaves information\n");
//  fprintf(stderr,"Recovering leaves bitmap\n");
//  strcpy(filename,basename);
//  strcat(filename,".il");
//  fprintf(stderr,"filename: %s\n",filename);
//  FILE * fi = fopen(filename,"r");
//  load(rep->il,fi);
//  //fclose(fi);  
//  rep->il_len = rep->il->n;
//  fprintf(stderr,"Bitmap recovered (len: %d bits)\n",rep->il_len);
//
//
//
////  strcpy(filename,basename);
////  strcat(filename,".il");
////  FILE * fi = fopen(filename,"r");
//  fread(&(rep->numberOfNodes),sizeof(uint),1,fi);
//  fprintf(stderr,"Number of nodes : %d\n",rep->numberOfNodes);
//  fread(&(rep->numberOfEdges),sizeof(uint),1,fi);
//  fprintf(stderr,"Number of edges : %d\n",rep->numberOfEdges);
//  fread(&(rep->maxLevel),sizeof(uint),1,fi);
//  fprintf(stderr,"Max Level : %d\n",rep->maxLevel);
//  fread(&(rep->nleaves),sizeof(uint),1,fi);
//  fprintf(stderr,"Leaves to read: %d\n",rep->nleaves);
//
//
//  fread(&(rep->idListSize),sizeof(uint),1,fi);
//  rep->idList = (uint *) malloc(sizeof(uint)*rep->idListSize);
//  fread(rep->idList,sizeof(uint),rep->idListSize,fi);
//
//
//	load(rep->bid,fi);
//	rep->bid_len = rep->bid->n;
////  rep->idListUniq = (uint *) malloc(sizeof(uint)*(rep->idListSize/W+1));
////	fread(rep->idListUniq,sizeof(uint),rep->idListSize/W+1,fi);
//
//  fread(&(rep->idListExtraSize),sizeof(uint),1,fi);
//  rep->idListExtra = (uint *) malloc(sizeof(uint)*rep->idListExtraSize);
//  fread(rep->idListExtra,sizeof(uint),rep->idListExtraSize,fi);
//  fclose(fi);
//
////  rep->leavesInf = (uint *)malloc(sizeof(uint)*(rep->nleaves*K*K/W+1));
//// 	fread(rep->leavesInf,sizeof(uint),rep->nleaves*K*K/W+1,fi);
// // fclose(fi);   
//	fprintf(stderr,"Leaves information recovered (len: %d bits)\n",rep->nleaves*K*K);
//	ttrep->info = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->info2[0] = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->info2[1] = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->element = (uint *)malloc(sizeof(uint)*MAX_INFO);	
//	ttrep->basex = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->basey = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->iniq = -1;
//	ttrep->finq =-1;
//	
//	trep->div_level_table = (uint *)malloc(sizeof(uint)*rep->maxLevel);
//	for(i=0;i<rep->maxLevel;i++)
//		rep->div_level_table[i]=exp_pow(K,rep->maxLevel-i);
	 free(filename);
	return trep;
}
*/

uint * compactTreeAdjacencyList(TREP * trep, int x){
	trep->info[0]=0;
	MREP * rep;
	uint j,i;
	uint xrelatIn = x/trep->tamSubm;//*trep->part;
	uint * listady;
	x = x%trep->tamSubm;
	for(i=0;i<trep->part;i++){
		rep=trep->submatrices[xrelatIn][i];
		trep->columna=i;
		listady = compactAdjacencyList(trep,rep,x);
	}
	return trep->info;
		
}

uint * compactTreeInverseList(TREP * trep, int y){
	trep->info[0]=0;
	MREP * rep;
	uint j,i;
	uint yrelatIn = y/trep->tamSubm;
	uint * listady;
	y = y%trep->tamSubm;
	for(i=0;i<trep->part;i++){
		rep=trep->submatrices[i][yrelatIn];
		trep->fila= i;
		listady = compactInverseList(trep,rep,y);
	}
	return trep->info;
}


void destroyTreeRepresentation(TREP *trep){
int i,j;
for(i=0;i<trep->part;i++){
	for(j=0;j<trep->part;j++)
		destroyRepresentation(trep->submatrices[i][j]);
	free(trep->submatrices[i]);
}
free(trep->submatrices);
free(trep->div_level_table1);
free(trep->div_level_table2);

free(trep->info2[0]);
free(trep->info2[1]);
free(trep->info);
free(trep->element);
free(trep->basex);
free(trep->basey);

free(trep->words);

free(trep);		
}

void   compressInformationLeaves(TREP * trep){
	//Recorrer todas las ils, calcular frecuencias y usar FT*
	MREP * rep;

	uint fila,columna;

	uint totalLeaves=0;
	uint i,j;
		
	for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){
			rep=trep->submatrices[fila][columna];
			if(trep->submatrices[fila][columna]->numberOfEdges!=0)
				totalLeaves +=rep->nleaves;	
		}
	}
	
	unsigned char * ilchar = (unsigned char *) malloc(sizeof(unsigned char)*totalLeaves*K2_3_char);
	
	unsigned char *aWord;
	unsigned int size;
	initialize(totalLeaves);
	//Creaci�n del vocabulario
	uint ilpos=0;
	uint jj;
	for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){
			rep=trep->submatrices[fila][columna];
			if(trep->submatrices[fila][columna]->numberOfEdges==0)
				continue;
		
			for(i=0;i<rep->nleaves;i++){
				//Usando una hash para el vocabulario...
				aWord=&(ilchar[ilpos]);  //the word parsed.
				
				for(j=0;j<K2_3;j++){
					if(bitget(rep->leavesInf,i*K2_3+j))
						bitsetchar(aWord,j);
					else
						bitcleanchar(aWord,j);
	    			}
				size = K2_3_char;
	      			j = search ((unsigned char *)aWord, size, &addrInTH );
	                        if (j==zeroNode) {
					insertElement ((unsigned char *) aWord, size, &addrInTH);
					hash[addrInTH].weight = 0;
					hash[addrInTH].size = 0;
					hash[addrInTH].len = K2_3_char;
					positionInTH[zeroNode] = addrInTH;
					zeroNode++;
				}

				hash[addrInTH].weight +=1;
			
				ilpos+=K2_3_char;		
			}
		}
	}		
	
	trep->zeroNode = zeroNode;
	trep->lenWords = K2_3_char;
	
	//Compresion de hojas
	
	int k=0;
	// Sorting the vocabulary by frequency.

	{	//Moves all the words with frequency = 1 to the end of the vocabulary.
		register int ii;
		register int kk;

	    kk=zeroNode-1;
	    ii=0;
	    while (ii<kk){
			while ((hash[positionInTH[ii]].weight!=1) && (ii <kk)) { ii++; }
			while ((hash[positionInTH[kk]].weight==1) && (ii <kk)) { kk--; }

			if (ii<kk){
				swap(&positionInTH[ii], &positionInTH[kk]);
				kk--;
				ii++;
			}
	  	}

		//k=ii; 
		k=ii+1; //the lenght of the vector to be sorted with qsort. So v[0 .. k-1]
	}

	//printf("Aplies qsort to the words with frequency > 1.\n");
	//Aplies qsort to the words with frequency > 1.
	qsort(positionInTH,k,sizeof(unsigned int),comparaFrecListaDesc);

	
	uint totalLeavesCount=0;
	for(i=0;i<zeroNode;i++){
		hash[positionInTH[i]].codeword = i;
		totalLeavesCount += hash[positionInTH[i]].weight;
	}
	//fprintf(stderr,"totalLeavesCount: %d, numberLeaves: %d\n",totalLeavesCount, numberLeaves);
	
	/********************** Beginning of the second pass **********************/
	
	uint icont=0;

	unsigned int codeword;
	unsigned int tam;
	

				//Compactando informaci�n de las hojas...

	ilpos=0;
	for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){
			rep=trep->submatrices[fila][columna];
			if(trep->submatrices[fila][columna]->numberOfEdges==0)
				continue;
			unsigned int tamTotal = 0;
			uint * listIL = (uint *) malloc(sizeof(uint)*rep->nleaves);
			uint listILCount =0;


			for(i=0;i<rep->nleaves;i++){	
				aWord=&(ilchar[ilpos]);  //the word parsed.
				size = K2_3_char;
				j = search ((unsigned char *)aWord, size, &addrInTH );
				listIL[listILCount++]=hash[addrInTH].codeword;
				ilpos+=K2_3_char;		
			}
			
			//fprintf(stderr,"tamTotal: %d\n",tamTotal);
			
			
			rep->compressIL = createFT(listIL,rep->nleaves);
			
			free(listIL);
		}
	}
	trep->words = (unsigned char *) malloc(sizeof(unsigned char)*trep->zeroNode*trep->lenWords);
	
	int wc = 0;
	for (i=0;i<zeroNode;i++){
		for(j=0;j<trep->lenWords;j++){
			trep->words[wc++]=hash[positionInTH[i]].word[j];
		}
	}

	free(ilchar);

}

uint compactCheckLinkQuery(TREP * trep,MREP * rep, uint p, uint q){
	int K1K1 = K1*K1;
	int K2K2 = K2*K2;

	if(rep->numberOfEdges==0)
		return 0;

	uint factorAdjust = K1K1/K2K2;

	uint nleaf,posInf,realvalue,  nleafrelat;
	uint prelat, qrelat;
	int i,node,div_level,xrelat;

	prelat = p;
	qrelat = q;

	node = 0;

	for(i=0;i<trep->maxLevel1-1;i++){

		div_level = trep->div_level_table1[i];


		node +=prelat/div_level*K1 + qrelat/div_level;

		prelat = prelat%div_level;
		qrelat = qrelat%div_level;

		if(isBitSet(rep->bt,node)==0)
			return 0;

		node = 	rank(rep->bt,node)*K1K1;

	}


	for(i=trep->maxLevel1-1;i<trep->maxLevel1;i++){

		div_level = trep->div_level_table1[i];

		node +=prelat/div_level*K1 + qrelat/div_level;
		prelat = prelat%div_level;
		qrelat = qrelat%div_level;


		if(isBitSet(rep->bt,node)==0)
			return 0;

		node = rep->cutBt + (rank(rep->bt,node)*K1K1-rep->cutBt)/factorAdjust;
	}

	uint cutPreRank = rank(rep->bt,rep->lastBt1_len-1);

		for(i=0;i<trep->maxLevel2-1;i++){

		div_level = trep->div_level_table2[i];

		node+=prelat/div_level*K2 + qrelat/div_level;

		prelat = prelat%div_level;
		qrelat = qrelat%div_level;


		if(isBitSet(rep->bt,node)==0)
			return 0;

		node =rep->cutBt + (rank(rep->bt,node-1)-cutPreRank)*K2K2;

	}


		nleaf = node-rep->bt_len;

		nleafrelat = nleaf + (prelat/K2_2)*K2+qrelat/K2_2;

		if(isBitSet(rep->bn,nleafrelat)){

			posInf = rank(rep->bn,nleafrelat);

			realvalue = accessFT(rep->compressIL,posInf);

			if(bitgetchar(&(trep->words[realvalue*trep->lenWords]),(qrelat%K2_2+(prelat%K2_2)*K2_2)))
				return 1;
			}

	return 0;

}


uint compactTreeCheckLinkQuery(TREP * trep, uint p, uint q){
	MREP * rep;

	rep=trep->submatrices[p/trep->tamSubm][q/trep->tamSubm];

	return compactCheckLinkQuery(trep,rep,p%trep->tamSubm,q%trep->tamSubm);

}


void saveTreeRep(TREP * trep, FILE * f){

  FILE * fv = f;


  fwrite(&(trep->part),sizeof(uint),1,fv);
  fwrite(&(trep->tamSubm),sizeof(uint),1,fv);

  fwrite(&(trep->numberOfNodes),sizeof(uint),1,fv);
  fwrite(&(trep->numberOfEdges),sizeof(uint),1,fv);
  
  fwrite(&(trep->repK1),sizeof(uint),1,fv);
  fwrite(&(trep->repK2),sizeof(uint),1,fv);
  fwrite(&(trep->maxRealLevel1),sizeof(uint),1,fv);
  fwrite(&(trep->maxLevel1),sizeof(uint),1,fv);
  fwrite(&(trep->maxLevel2),sizeof(uint),1,fv);

//  fwrite(&(trep->maxLevel),sizeof(uint),1,fv);
//  
  fwrite(&trep->zeroNode,sizeof(uint),1,fv); //stores the number of words of the vocabulary
	fwrite(&trep->lenWords,sizeof(uint),1,fv);
	//Writes the vocabulary to disk.
	int i;
	fwrite(trep->words, sizeof(char), trep->lenWords * trep->zeroNode, fv);
//	for (i=0;i<zeroNode;i++)
//		fwrite(hash[positionInTH[i]].word,sizeof(char),trep->lenWords,fv);


  FILE * fi = f;

 
  uint part= trep->part;
	int fila,columna;
	MREP* rep;
	for(fila=0;fila<part;fila++){
		for(columna=0;columna<part;columna++){  
		  rep=trep->submatrices[fila][columna];
		//  fclose(fi);
		//  
		//  strcpy(filename,argv[3]);
		//  strcat(filename,".rs");
		//  FILE * fr = fopen(filename,"w");
		  
		  //fprintf(stderr,"fila: %d, columna: %d, edges: %d\n",fila,columna,rep->numberOfEdges);
		  fwrite(&(rep->numberOfNodes),sizeof(uint),1,fi);
		  fwrite(&(rep->numberOfEdges),sizeof(uint),1,fi);
 			if(trep->submatrices[fila][columna]->numberOfEdges==0)
				continue;
		  fwrite(&(rep->cutBt),sizeof(uint),1,fi);
			fwrite(&(rep->lastBt1_len),sizeof(uint),1,fi);
		  //fwrite(&(rep->maxLevel),sizeof(uint),1,fi);
		  fwrite(&(rep->nleaves),sizeof(uint),1,fi);
		  //fwrite (rep->leavesInf,sizeof(uint),rep->nleaves*K2*K2/W+1,fi);
		  saveFT(rep->compressIL,fi);
		}
	}
 
  FILE * ft = f;
	for(fila=0;fila<part;fila++){
		for(columna=0;columna<part;columna++){
			if(trep->submatrices[fila][columna]->numberOfEdges!=0)
				save(trep->submatrices[fila][columna]->bt,ft);
		}
	}
  
  FILE * fn = f;
	for(fila=0;fila<part;fila++){
		for(columna=0;columna<part;columna++){
				if(trep->submatrices[fila][columna]->numberOfEdges!=0)
				  save(trep->submatrices[fila][columna]->bn,fn);
			}
		}
  
}

TREP * loadTreeRepresentation(FILE * f) {
	TREP * trep;
	MREP * rep;
	int i,j,k;
	
		trep = malloc(sizeof(struct treeRep));

//    trep->maxLevel=maxlevel;
//    trep->div_level_table;
    
//    ttrep->info;
//    ttrep->info2[2];
//    
//    ttrep->element;
//    ttrep->basex;
//    ttrep->basey;
//    ttrep->iniq;
//    ttrep->finq;
    
  FILE * fv = f;

  fread(&(trep->part),sizeof(uint),1,fv);
  fread(&(trep->tamSubm),sizeof(uint),1,fv);

  fread(&(trep->numberOfNodes),sizeof(uint),1,fv);
  fread(&(trep->numberOfEdges),sizeof(uint),1,fv);
	
	
	fread(&(trep->repK1),sizeof(uint),1,fv);
  //fprintf(stderr,"K1 : %d\n",trep->repK1);
  fread(&(trep->repK2),sizeof(uint),1,fv);
  //fprintf(stderr,"K2 : %d\n",trep->repK2);
  //K2 = trep->K2;
  //K1 = trep->K1;
  fread(&(trep->maxRealLevel1),sizeof(uint),1,fv);
  //fprintf(stderr,"Max Real Level1 : %d\n",trep->maxRealLevel1);
  fread(&(trep->maxLevel1),sizeof(uint),1,fv);
  //fprintf(stderr,"Max Level1 : %d\n",trep->maxLevel1);
  fread(&(trep->maxLevel2),sizeof(uint),1,fv);
  //fprintf(stderr,"Max Level2 : %d\n",trep->maxLevel2);

	
//	fread(&(trep->maxLevel),sizeof(uint),1,fv);
	
	fread(&trep->zeroNode,sizeof(uint),1,fv); //reads the number of words of the vocabulary
	fread(&trep->lenWords,sizeof(uint),1,fv);
	//fprintf(stderr,"zeroN:%d, len:%d\n",trep->zeroNode,trep->lenWords);
	trep->words = (unsigned char *) malloc(sizeof(char)*trep->lenWords*trep->zeroNode);
	fread(trep->words,sizeof(unsigned char),trep->lenWords*trep->zeroNode,fv);

	
	
	trep->div_level_table1 = (uint *)malloc(sizeof(uint)*trep->maxLevel1);
	for(i=0;i<trep->maxLevel1;i++)
		trep->div_level_table1[i]=exp_pow(K1,trep->maxRealLevel1-i);
	
	trep->div_level_table2 = (uint *)malloc(sizeof(uint)*trep->maxLevel2);
	for(i=0;i<trep->maxLevel2;i++)
		trep->div_level_table2[i]=exp_pow(K2,trep->maxLevel2+L-i);

	
	//fprintf(stderr,"part: %d subm: %d nodes: %u edges: %d\n",trep->part,trep->tamSubm,trep->numberOfNodes,trep->numberOfEdges);



	trep->info = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->info2[0] = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->info2[1] = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->element = (uint *)malloc(sizeof(uint)*MAX_INFO);	
	trep->basex = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->basey = (uint *)malloc(sizeof(uint)*MAX_INFO);
	trep->iniq = -1;
	trep->finq =-1;

    trep->submatrices=(MREP ***)malloc(sizeof(MREP **)*trep->part);
    
    int fila,columna;
    for(i=0;i<trep->part;i++){
    	trep->submatrices[i]=(MREP **)malloc(sizeof(MREP *)*trep->part);
    }

	for(i=0;i<trep->part;i++){
		for(j=0;j<trep->part;j++){
			rep = (MREP *) malloc(sizeof(struct matrixRep));
			rep->bt = (bitRankW32Int *) malloc(sizeof(struct sbitRankW32Int));
			rep->bn = (bitRankW32Int *) malloc(sizeof(struct sbitRankW32Int));				
			trep->submatrices[i][j]=rep;
				}
	}
   
  FILE * fi = f;
 
	int totalnumberLeaves=0;

	for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){  
 			//fprintf(stderr,"IL: fila= %d, columna=%d, aristas desde %d y desde %d, (%2.2f\%)\n",fila,columna,fila*trep->tamSubm, columna*trep->tamSubm,(float)(fila*trep->part+columna)*100/trep->part/trep->part);
	
		  rep=trep->submatrices[fila][columna];
  		fread(&(rep->numberOfNodes),sizeof(uint),1,fi);
	//	  fprintf(stderr,"Number of nodes : %d\n",rep->numberOfNodes);
		  fread(&(rep->numberOfEdges),sizeof(uint),1,fi);
		  //fprintf(stderr,"Number of edges : %d\n",rep->numberOfEdges);
			if(rep->numberOfEdges==0)
				continue;

      fread(&(rep->cutBt),sizeof(uint),1,fi);

	    fread(&(rep->lastBt1_len),sizeof(uint),1,fi);

	
	//	  fprintf(stderr,"Max Level : %d\n",rep->maxLevel);
		  fread(&(rep->nleaves),sizeof(uint),1,fi);
	//	  fprintf(stderr,"Leaves to read: %d\n",rep->nleaves);
		
	//	  fprintf(stderr,"Leaves to read: %d\n",rep->nleaves);
		//rep->leavesInf = (uint *)malloc(sizeof(uint)*(rep->nleaves*K2*K2/W+1));
		//fread(rep->leavesInf,sizeof(uint),rep->nleaves*K2*K2/W+1,fi);
		rep->compressIL = loadFT(fi);
		totalnumberLeaves+=rep->compressIL->listLength;  
		rep->leavesInf=NULL;
		  
		  
		}
	}
  
  //fprintf(stderr,"totalnumberLeaves: %d\n",totalnumberLeaves);
	
	//fprintf(stderr,"Recovering tree bitmap\n");
  FILE * ft = f;
	for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){  
 			//fprintf(stderr,"TR: fila= %d, columna=%d, aristas desde %d y desde %d, (%2.2f\%)\n",fila,columna,fila*trep->tamSubm, columna*trep->tamSubm,(float)(fila*trep->part+columna)*100/trep->part/trep->part);
	
		  rep=trep->submatrices[fila][columna];
			if(rep->numberOfEdges==0)
				continue;
  		load(rep->bt,ft);
  		rep->bt_len = rep->bt->n;
  
		}
	}
  //fprintf(stderr,"Bitmap recovered (len: %d bits)\n",rep->bt_len);
    
  //fprintf(stderr,"Recovering leaves bitmap\n");
  FILE * fn = f;
  for(fila=0;fila<trep->part;fila++){
		for(columna=0;columna<trep->part;columna++){  
 			//fprintf(stderr,"LV: fila= %d, columna=%d, aristas desde %d y desde %d, (%2.2f\%)\n",fila,columna,fila*trep->tamSubm, columna*trep->tamSubm,(float)(fila*trep->part+columna)*100/trep->part/trep->part);
	
		  rep=trep->submatrices[fila][columna];
		  
		  if(rep->numberOfEdges==0)
				continue;
		  load(rep->bn,fn);
		  rep->bn_len = rep->bn->n;
		}
	}
  
  

  
//  fprintf(stderr,"Bitmap recovered (len: %d bits)\n",rep->bn_len);
//    
//  fprintf(stderr,"Recovering leaves information\n");
//  fprintf(stderr,"Recovering leaves bitmap\n");
//  strcpy(filename,basename);
//  strcat(filename,".il");
//  fprintf(stderr,"filename: %s\n",filename);
//  FILE * fi = fopen(filename,"r");
//  load(rep->il,fi);
//  //fclose(fi);  
//  rep->il_len = rep->il->n;
//  fprintf(stderr,"Bitmap recovered (len: %d bits)\n",rep->il_len);
//
//
//
////  strcpy(filename,basename);
////  strcat(filename,".il");
////  FILE * fi = fopen(filename,"r");
//  fread(&(rep->numberOfNodes),sizeof(uint),1,fi);
//  fprintf(stderr,"Number of nodes : %d\n",rep->numberOfNodes);
//  fread(&(rep->numberOfEdges),sizeof(uint),1,fi);
//  fprintf(stderr,"Number of edges : %d\n",rep->numberOfEdges);
//  fread(&(rep->maxLevel),sizeof(uint),1,fi);
//  fprintf(stderr,"Max Level : %d\n",rep->maxLevel);
//  fread(&(rep->nleaves),sizeof(uint),1,fi);
//  fprintf(stderr,"Leaves to read: %d\n",rep->nleaves);
//
//
//  fread(&(rep->idListSize),sizeof(uint),1,fi);
//  rep->idList = (uint *) malloc(sizeof(uint)*rep->idListSize);
//  fread(rep->idList,sizeof(uint),rep->idListSize,fi);
//
//
//	load(rep->bid,fi);
//	rep->bid_len = rep->bid->n;
////  rep->idListUniq = (uint *) malloc(sizeof(uint)*(rep->idListSize/W+1));
////	fread(rep->idListUniq,sizeof(uint),rep->idListSize/W+1,fi);
//
//  fread(&(rep->idListExtraSize),sizeof(uint),1,fi);
//  rep->idListExtra = (uint *) malloc(sizeof(uint)*rep->idListExtraSize);
//  fread(rep->idListExtra,sizeof(uint),rep->idListExtraSize,fi);
//  fclose(fi);
//
////  rep->leavesInf = (uint *)malloc(sizeof(uint)*(rep->nleaves*K*K/W+1));
//// 	fread(rep->leavesInf,sizeof(uint),rep->nleaves*K*K/W+1,fi);
// // fclose(fi);   
//	fprintf(stderr,"Leaves information recovered (len: %d bits)\n",rep->nleaves*K*K);
//	ttrep->info = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->info2[0] = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->info2[1] = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->element = (uint *)malloc(sizeof(uint)*MAX_INFO);	
//	ttrep->basex = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->basey = (uint *)malloc(sizeof(uint)*MAX_INFO);
//	ttrep->iniq = -1;
//	ttrep->finq =-1;
//	
//	trep->div_level_table = (uint *)malloc(sizeof(uint)*rep->maxLevel);
//	for(i=0;i<rep->maxLevel;i++)
//		rep->div_level_table[i]=exp_pow(K,rep->maxLevel-i);
	return trep;
}

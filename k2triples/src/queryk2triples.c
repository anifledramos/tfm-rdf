#include <stdio.h>
#include <math.h>
#include <string.h>
#include "kTree.h"
#include <sys/time.h>
#include <time.h>
#include "directcodesI.h"
#include "commons.h"
#include "simplePatterns.h"
#include "k2triples-ops.h"
#define SEC_TIME_DIVIDER  ((double) 1.0    )     //1 sec = 1 sec
#define SEC_TIME_UNIT       "sec"


int main(int argc, char* argv[]) {
	int i,k;
	struct timeval tv1, tv2;
	FILE * queries,* resultado;
	FILE * f;

	int * queriesDat[4];
	int tipoQuery;

	if (argc < 6) {
		fprintf(
			stderr,
			"USAGE: %s <GRAPH> <queries> <nQueries> <|SO|> <queryResultados>\n",
			argv[0]);
		return (-1);
	}

	inicializaMemoria();

	int numSO=atoi(argv[4]);
	int numQueries = atoi(argv[3]);

	K2TRIPLES *k2triples = loadK2TRIPLES(argv[1]);
	k2triples->nso = numSO;

	inicializarEstructuras(k2triples->npreds, k2triples->nso);
	
	for (i = 0; i < 4; i++) {
		queriesDat[i] = (int *) malloc(sizeof(int) * numQueries);
	}

	if ((queries = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "error: no se pudo abrir el fichero %s\n", argv[2]);
		return 1;
	} else {
	}


	if ((resultado = fopen(argv[5], "w+")) == NULL) {
		fprintf(stderr, "error: no se pudo abrir el fichero %s\n", argv[5]);
		return 1;
	} else {
	}

	foutput=resultado;

	char nomFich[256];

	int totalAristasCalc=0;

	for (i=1;i<=k2triples->npreds;i++){
		totalAristasCalc+=k2triples->trees[i]->numberOfEdges;
	}

	fprintf(stderr,"Carga finalizada....\n");
	int results=0;
	int errores = 0;

	gettimeofday(&tv1, NULL);
	startClock();
	
	char line[1024];
	char seps[]   = " ,\t";
	char * token;
	fprintf(stderr,"Iniciando consultas....\n");
	for (i = 0; i < numQueries; i++) {
		int qs1,qp1,qo1,qs2,qp2,qo2;	
		
		if (-1==fgets(line,1024,queries)){
			fprintf(stderr,"error al leer queries\n");
			exit(1);
		}
	
		//SEPARAMOS EN TOKENS
		token = strtok (line, seps);
		sscanf(token,"%d",&qs1);
		token = strtok (NULL, seps);
		sscanf(token,"%d",&qp1);
		token = strtok (NULL, seps);
		sscanf(token,"%d",&qo1);
		token = strtok (NULL, seps);
		
		//JOINS	
		if (token != NULL)
		{
			sscanf(token,"%d",&qs2);
			token = strtok (NULL, seps);
			sscanf(token,"%d",&qp2);
			token = strtok (NULL, seps);
			sscanf(token,"%d",&qo2);
			token = strtok (NULL, seps);
			fprintf(resultado,"%d %d %d - %d %d %d\n",qs1,qp1,qo1,qs2,qp2,qo2);
	

			//1. Calcular PORSUJETO-POROBJETO y variables v1-v2-v3-v4
			int joinA=qs1<0?PORSUJETO:POROBJETO;
			int joinB=qs2<0?PORSUJETO:POROBJETO;
		 	
	
			int v1=qs1<0?qo1:qs1;
		        int v2=qp1;
			int v3=qs2<0?qo2:qs2;
			int v4=qp2;

			//2. Normalizar: poner el primer patrón como el más definido
			int peso1=(v1>0?2:0)+(v2>0?1:0);
			int peso2=(v3>0?2:0)+(v4>0?1:0);
		
			int invertido=0;
			if (peso2>peso1){
			   int pSwap=v2;v2=v4;v4=pSwap;
			   int sSwap=v1;v1=v3;v3=sSwap;	
			   int joinSwap=joinA;joinA=joinB;joinB=joinSwap;
			   invertido=1;
			} 
	
			//3. Elegir indices DAC en función del tipo de join
		//	INDICEDAC * indice1=(joinA==PORSUJETO)?indiceOP:indiceSP;
		//	INDICEDAC * indice2=(joinB==PORSUJETO)?indiceOP:indiceSP;


			//4. Calcular la función que toca
			int peso=0;

			if (v1>0) peso+=8;if (v2>0) peso+=4;if (v3>0) peso+=2;if (v4>0) peso+=1;
			v1--;v3--;
			switch (peso){
			/*	//JoinA
				case 15:
					sppoIzquierda(treps,numPredicados,v1,v2,v4,v3,joinA,joinB);
					break;
				//Join B
				case 14:
					sp_oParalelo(treps,numPredicados,indice1,indice2,v1,v2,v3,1,joinA,joinB);
					break;
				//Join C
				case 10:
				 	s__oParalelo(treps,indice1,indice2,numPredicados,v1,v3,1,joinA,joinB);
					break;
				//Join D
				case 13:
					_ppoDerecha(treps,v4,v2,v1,joinB,joinA);
					break;
				//Join E1
				case 6:
					_p_oIzquierda(treps, numPredicados, indice2, indice1, v4, v1,1,joinB,joinA);
					break;
				//Join E2
				case 12:
					__poDerecha(treps,numPredicados,indice1, indice2,v1,v2,joinA,joinB);
					break;
				//Join F
				case 8:
					___oDerecha(treps,numPredicados,indice1,indice2, v1,1,joinA, joinB);
					break;
				//Join G
				case 5:
					_pp_Interactiva(treps, v2, v3, joinA, joinB);
					break;
				//Join H
				case 4:
					_p__Izquierda(treps,v2,numPredicados, indice1, indice2,1,joinA,joinB);
					break;*/
				default: 
					fprintf(stderr,"Operación no soportada\n");
					break;
			}
			
		}
		//PATRONES SIMPLES
		else{
		  //Patrones simples
	          fprintf(resultado,"%d %d %d\n",qs1,qp1,qo1);
		  int numPatron=0;
		  if (qs1>0) numPatron+=4;
		  if (qp1>0) numPatron+=2;
		  if (qo1>0) numPatron+=1;
		  
		  switch (numPatron){

			case 7:
				results = spoF(resultado,k2triples,qs1-1,qp1,qo1-1);
				break;
			case 6:
				results = spF(resultado,k2triples, qs1-1,qp1);
				break;
			case 3:
				results = poF(resultado,k2triples, qp1,qo1-1);
				break;
			case 5:
				results = soDobleIndexF(resultado,k2triples,qs1-1,qo1-1);
				break;
			case 4:
				results = sIndexF(resultado,k2triples, qs1-1);
				break;
			case 2:	
				results = pOrdenadoF(resultado,k2triples, qp1);
				break;
			case 1:
				results = oIndexF(resultado,k2triples, qo1-1);
				break;
			default:
				fprintf(stderr, "error\n");
		  }	
		}


	}	
	fprintf(stderr,"Fin de ejecución de las consultas...\n");
	fclose(resultado);
	gettimeofday(&tv2, NULL);
	stopClock();
	fprintf(
		stderr,
		"Tiempo total: %ld en us\n",
		(tv2.tv_sec - tv1.tv_sec) * 1000000
		+ (tv2.tv_usec - tv1.tv_usec));

	return 0;
}


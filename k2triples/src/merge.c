#include "merge.h"

int cmp(const void *a, const void *b) {
	return ((*((Result2**) b))[0].value - (*((Result2**) a))[0].value);
}

void merge(Result2 *v1, Result2 *v2, Result2 *vres)
{
	unsigned int left = 1, right = 1;
	unsigned int moved = 1;

	// Se busca, para cada elemento en v1, la posición en la que
	// se inserta en vres de acuerdo a los elementos en v2
	unsigned int i = 1;
	for (i = 1; i <= v1[0].value; i++)
	{
		// Itero hasta que encuentro algo mayor que el valor de v1[i]
		while(right <= v2[0].value)
		{
			if (v1[i].value <= v2[right].value) break;
			right++;
		}

		// Copio los v2
		unsigned int copy = right-left;
		memcpy(vres + moved, v2 + left, copy * sizeof(Result2));
		moved += copy;
		left = right;

		if (right > v2[0].value)
		{
			// LO ACABÉ!
			unsigned int copy = v1[0].value-i+1;
			memcpy(vres + moved, v1 + i, copy * sizeof(Result2));
			moved += copy;
			break;
		}
		else
		{
			// Compuebo si era igual
			if (v1[i].value == v2[right].value)
			{
				unsigned int n_sources = v1[i].sources[0]+ v2[right].sources[0];
				v1[i].sources = (unsigned int*) realloc(v1[i].sources, (n_sources + 1) * sizeof(unsigned int));
				memcpy(v1[i].sources + v1[i].sources[0] + 1, v2[right].sources + 1, v2[right].sources[0] * sizeof(unsigned int));
				v1[i].sources[0] = n_sources;
				free(v2[right].sources);

				right++;
				left = right;
			}
			
			vres[moved] = v1[i];
			moved++;
		}
	}

	if (right <= v2[0].value) 
	{
		unsigned int copy = v2[0].value-left+1;
		memcpy(vres + moved, v2 + left, copy * sizeof(Result2));
		moved += copy;
	}

	vres[0].value = moved-1;
}

void mergeMiguel(Result2 **vectores, int numVectores) {
	if (numVectores == 1) return;

	// Ordenamos los vectores de acuerdo a su número de elementos
	qsort(vectores, numVectores, sizeof(Result2**), &cmp);

	int p1 = numVectores-1; // ptr a la última posición ordenada
	int p2 = numVectores;   // ptr a la última posición 'nueva'
	unsigned int l = 0;	// número de cadenas 'nuevas'
	unsigned int vf1,vf2;	// vectores fusionados en cada paso
	
	// Inicializo la posición extra
	Result2 extra = {NULL, vectores[0][0].value};
	vectores[p2] = (Result2*)malloc(1*sizeof(Result2));
	vectores[p2][0] = extra;
	
	while (1) {
		Result2 *v1,*v2;

		// Identificamos los elementos que van a ser fusionados
		if (l == 0)
		{
			// No hay elementos 'nuevos': se eligen los dos (últimos) ordenados
			v1 = vectores[p1]; 
			vf1=p1; p1--;
			v2 = vectores[p1]; 
			vf2=p1; p1--;
		}
		else
		{
			if (vectores[p1][0].value <= vectores[p2][0].value)
			{
				// El último elemento 'ordenado' es más pequeño
				v1 = vectores[p1]; 
				vf1=p1; p1--;
			}
			else
			{
				// El último elemento 'nuevo' es más pequeño
				v1 = vectores[p2]; 
				vf1=p2; p2--;
				l--;
			}

			if (l == 0)
			{
				// No hay elementos 'nuevos': se eligen el (último) ordenado
				v2 = vectores[p1]; 
				vf2=p1; p1--;
			}
			else
			{
				if (p1 < 0)
				{
					// No hay elementos 'ordenados': se eligen el (último) nuevo
					v2 = vectores[p2]; 
					vf2=p2; p2--;
					l--;
				}
				else
				{
					if (vectores[p1][0].value <= vectores[p2][0].value)
					{
						// El último elemento 'ordenado' es más pequeño
						v2 = vectores[p1]; 
						vf2=p1; p1--;
					}
					else
					{
						// El último elemento 'nuevo' es más pequeño
						v2 = vectores[p2]; 
						vf2=p2; p2--;
						l--;
					}
				}
			}
		}

		// Creamos un nuevo vector para hacer el merge de los dos
		// menos frecuentes
		unsigned int size = v1[0].value + v2[0].value;
		Result2 begin = { NULL, size };
		Result2 *vres = (Result2*) malloc((size + 1) * sizeof(Result2));
		vres[0] = begin;

		merge(v1,v2,vres);

		free(vectores[vf1]); vectores[vf1] = &extra;
		free(vectores[vf2]); vectores[vf2] = &extra;

		if ((p1 < 0) && (l == 0)) {
			vectores[0] = vres;
			break;
		}

		if (p1 < 0)
		{
			// Entra en los ordenados
			p1++;
			vectores[p1] = vres;
		}
		else
		{
			if (vres[0].value <= vectores[p1][0].value)
			{
				// Entra en los ordenados
				p1++;
				vectores[p1] = vres;
			}
			else
			{
				if (vres[0].value >= vectores[p2][0].value)
				{
					// Entra en los nuevos
					vectores[p2-l] = vres;
					l++;
				}
				else
				{
					// ** Caso especial **
					vectores[p1+1] = vectores[p1];
					vectores[p1] = vres;
					p1++;					
				}
			}
		}
	}
}



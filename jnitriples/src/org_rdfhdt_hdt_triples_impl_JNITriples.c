#include "org_rdfhdt_hdt_triples_impl_JNITriples.h"

#include "k2triples-common.h"
#include "k2triples-builder.h"
#include "k2triples-ops.h"
#include "simplePatterns.h"

#include <stdio.h>

jintArray buildJNIArray(JNIEnv * env, int * orig);
jobjectArray buildJNIBidiArray(JNIEnv * env, int ** orig);

int MAXTRIPLEBUFFER = 50000;

void correctEndianness(FILE * f) {
	fseek(f, 0, SEEK_SET);
	int num = 1;
	if (*(char *)&num == 1) {
		int numEnteros;
		int buffer[MAXTRIPLEBUFFER * 3];
		/* int posRead = 0; */
		uint lastRead = 0;
		do {
			fseek(f, lastRead, SEEK_SET);
			numEnteros = fread(buffer, sizeof(int), MAXTRIPLEBUFFER * 3, f);
			int i;
			for (i = 0; i < numEnteros; i++) {
				uint base = buffer[i];
				buffer[i] = ((base>>24)&0xff) | // move byte 3 to byte 0
	                    ((base<<8)&0xff0000) | // move byte 1 to byte 2
	                    ((base>>8)&0xff00) | // move byte 2 to byte 1
	                    ((base<<24)&0xff000000); // byte 0 to byte 3
			}
			fseek(f, lastRead, SEEK_SET);
			fwrite(buffer, sizeof(int), numEnteros, f);
			lastRead += numEnteros * sizeof(int);
		} while (numEnteros > 0);
	}
	fseek(f, 0, SEEK_SET);
}
/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _createJNITriples
 * Signature: (Ljava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1createJNITriples
  (JNIEnv * env, jobject obj, jstring filename, jint npreds, jint nso) {

	const char *inFile = (*env)->GetStringUTFChars(env, filename, 0);
	
	FILE * f = fopen(inFile, "rb+");

	correctEndianness(f);

	K2TRIPLES * k2triples = createK2TRIPLES(f);
	k2triples->npreds = npreds;
	k2triples->nso = nso;

	fclose(f);

	inicializarEstructuras(k2triples->npreds, k2triples->nso);

	return (jlong) k2triples;
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _loadJNITriples
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1loadJNITriples
  (JNIEnv * env, jobject obj, jstring filename) {

	const char *inFile = (*env)->GetStringUTFChars(env, filename, 0);

	FILE * f = fopen(inFile, "rb+");

	printf("Loading from %s\n", inFile);

//	correctEndianness(f);

	K2TRIPLES * k2triples = loadK2TRIPLES(f);

	inicializarEstructuras(k2triples->npreds, k2triples->nso);

	fclose(f);

	return (jlong) k2triples;
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _storeJNITriples
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1storeJNITriples
  (JNIEnv * env, jobject obj, jlong _k2triples, jstring filename) {

	const char *outFile = (*env)->GetStringUTFChars(env, filename, 0);

	K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;

	FILE * out = fopen(outFile, "wb+");

	storeK2TRIPLES(k2triples, out);

//	correctEndianness(out);

	fclose(out);

	return;
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _destroyJNITriples
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1destroyJNITriples
  (JNIEnv * env, jobject obj, jlong _k2triples) {

	K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;

	destroyK2TRIPLES(k2triples);

	return;
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _sizeJNITriples
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1sizeJNITriples
  (JNIEnv * env, jobject obj, jlong _k2triples) {

	K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;

	return (jlong) sizeK2TRIPLES(k2triples);
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _spo
 * Signature: (JIII)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1spo
  (JNIEnv * env, jobject obj, jlong _k2triples, jint subject, jint predicate, jint object) {

	K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;

	return (jint) spo(k2triples, (int) subject, (int) predicate, (int) object);

}

int DEF_RES= 1000000;
jint * results = NULL;

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    doSearchJNITriples
 * Signature: (JIII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_doSearchJNITriples
	(JNIEnv * env, jobject obj, jlong _k2triples, jint subject, jint predicate, jint object) {
/*
	K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;

	if (results == null) {
		jint * results = (jint *) malloc(DEF_RES * sizeof(int));
	}


	if (subject >= 0) {
		if (predicate >= 0) {
			if (object >= 0) {
				//SPO
				int len = spo(k2triples, subject, predicate, object, &results);
			} else {
				//SP?
				results = sp(k2triples,subject, predicate);
			}
		}
	}
	int nres = results[0];
	int LEN = 1 + 3 * nres;
	jintArray array = (*env)->NewIntArray(env, LEN);
	(*env)->SetIntArrayRegion(env, array, 0 , LEN, results);

	return array;
	*/
return 0;

}



/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchSPO
 * Signature: (JIII)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchSPO
	(JNIEnv * env, jobject obj, jlong _k2triples, jint subject, jint predicate, jint object) {
		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		return spo(k2triples, subject, predicate, object);
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchSP
 * Signature: (JII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchSP
	(JNIEnv * env, jobject obj, jlong _k2triples, jint subject, jint predicate) {

		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		int * results = sp(k2triples, subject, predicate);
		return buildJNIArray(env, results);
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchPO
 * Signature: (JII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchPO
	(JNIEnv * env, jobject obj, jlong _k2triples, jint predicate, jint object) {
		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		int * results = po(k2triples, predicate, object);
		return buildJNIArray(env, results);
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchSO
 * Signature: (JII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchSO
	(JNIEnv * env, jobject obj, jlong _k2triples, jint subject, jint object) {
		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		int * results = so(k2triples, subject, object);
		return buildJNIArray(env, results);
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchS
 * Signature: (JI)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchS
	(JNIEnv * env, jobject obj, jlong _k2triples, jint subject) {
		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		int ** results = s(k2triples, subject);
		return buildJNIBidiArray(env, results);
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchP
 * Signature: (JI)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchP
	(JNIEnv * env, jobject obj, jlong _k2triples, jint predicate) {
		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		int ** results = p(k2triples, predicate);
		return buildJNIBidiArray(env, results);
}
/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchO
 * Signature: (JI)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchO
	(JNIEnv * env, jobject obj, jlong _k2triples, jint object) {
		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		int ** results = o(k2triples, object);
		return buildJNIBidiArray(env, results);
}

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    getNPreds
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_getNPreds
	(JNIEnv * env, jobject obj, jlong _k2triples) {
		K2TRIPLES * k2triples = (K2TRIPLES *) _k2triples;
		return k2triples->npreds;
}

jintArray buildJNIArray(JNIEnv * env, int * orig) {
	int nres = orig[0];
	int LEN = 1 + nres;
	jintArray array = (*env)->NewIntArray(env, LEN);
	(*env)->SetIntArrayRegion(env, array, 0 , LEN, orig);
	return array;
}

jobjectArray buildJNIBidiArray(JNIEnv * env, int ** orig) {

	// Get the float array class
  jclass intArrayClass = (*env)->FindClass(env, "[I");

  // Check if we properly got the float array class
  if (intArrayClass == NULL)
  {
      // Ooops
      return NULL;
  }

  // Create the returnable 2D array
  jobjectArray returnArray = (*env)->NewObjectArray(env, (jsize) 2, intArrayClass, NULL);

	int LEN = orig[0][0] + 1;
	int i;
	for (i = 0; i < 2; i++) {
		jintArray intArray = (*env)->NewIntArray(env, LEN);
	  (*env)->SetIntArrayRegion(env, intArray, (jsize) 0, (jsize) LEN, (jint*) orig[i]);
	  (*env)->SetObjectArrayElement(env, returnArray, (jsize) i, intArray);
	  (*env)->DeleteLocalRef(env, intArray);
	}

	return returnArray;
}

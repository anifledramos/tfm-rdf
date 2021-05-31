/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_rdfhdt_hdt_triples_impl_JNITriples */

#ifndef _Included_org_rdfhdt_hdt_triples_impl_JNITriples
#define _Included_org_rdfhdt_hdt_triples_impl_JNITriples
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _createJNITriples
 * Signature: (Ljava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1createJNITriples
  (JNIEnv *, jobject, jstring, jint, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _loadJNITriples
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1loadJNITriples
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _storeJNITriples
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1storeJNITriples
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _destroyJNITriples
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1destroyJNITriples
  (JNIEnv *, jobject, jlong);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    _sizeJNITriples
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples__1sizeJNITriples
  (JNIEnv *, jobject, jlong);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    doSearchJNITriples
 * Signature: (JIII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_doSearchJNITriples
  (JNIEnv *, jobject, jlong, jint, jint, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchSPO
 * Signature: (JIII)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchSPO
  (JNIEnv *, jobject, jlong, jint, jint, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchSP
 * Signature: (JII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchSP
  (JNIEnv *, jobject, jlong, jint, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchPO
 * Signature: (JII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchPO
  (JNIEnv *, jobject, jlong, jint, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchSO
 * Signature: (JII)[I
 */
JNIEXPORT jintArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchSO
  (JNIEnv *, jobject, jlong, jint, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchS
 * Signature: (JI)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchS
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchP
 * Signature: (JI)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchP
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    searchO
 * Signature: (JI)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_searchO
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     org_rdfhdt_hdt_triples_impl_JNITriples
 * Method:    getNPreds
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_triples_impl_JNITriples_getNPreds
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif

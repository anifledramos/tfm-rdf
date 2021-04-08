#include "org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.h"

#include "../libCSD/StringDictionaryPFC.h"

#include <stdio.h>

JNIEXPORT jstring JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1writeJNIDictionary
  (JNIEnv * env, jobject obj, jstring filename){
    printf("Generating JNI Dictionary !!!! \n");
    return filename;
  }

JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1createJNIDictionary
  (JNIEnv * env, jobject obj, jobject it, jint bucketsize){
    
    StringDictionaryPFC(it, bucketsize);

  }
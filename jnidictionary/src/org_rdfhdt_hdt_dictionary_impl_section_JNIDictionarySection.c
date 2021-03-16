#include "org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.h"

#include <stdio.h>

JNIEXPORT jstring JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1writeJNIDictionary
  (JNIEnv * env, jobject obj, jstring filename){
    printf("Generating JNI Dictionary !!!! \n");
    return filename;
  }

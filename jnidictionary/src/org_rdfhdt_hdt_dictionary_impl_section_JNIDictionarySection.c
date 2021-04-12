#include "org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.h"

#include "../libCSD/StringDictionaryPFC.h"
#include "../libCSD/iterators/IteratorDictStringPlain.h"

#include <stdio.h>

JNIEXPORT jstring JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1writeJNIDictionary
  (JNIEnv * env, jobject obj, jstring filename){
    printf("Generating JNI Dictionary !!!! \n");
    return filename;
  }

JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1createJNIDictionary
  (JNIEnv * env, jobject obj, jbyteArray arr, jint bucketsize){
    
    uint lenStr = sizeof(arr);

    jboolean *isCopy = JNI_FALSE;
    jbyte* b = env->GetByteArrayElements( arr, isCopy);
    uchar *str = (uchar*)b;

    IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
    StringDictionaryPFC(it, bucketsize);
    printf("JNIDictionary created successfully!!!! \n");

    env->ReleaseByteArrayElements( arr, b, 0);
    
  }
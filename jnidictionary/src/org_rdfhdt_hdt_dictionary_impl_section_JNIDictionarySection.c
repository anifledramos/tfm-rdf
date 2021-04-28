#include "org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.h"

#include "../libCSD/StringDictionaryPFC.h"
#include "../libCSD/iterators/IteratorDictStringPlain.h"

#include <stdio.h>

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    _writeJNIDictionary
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1writeJNIDictionary
  (JNIEnv * env, jobject obj, jstring filename){
    printf("Generating JNI Dictionary !!!! \n");
    return filename;
  }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    _createJNIDictionary
 * Signature: ([BI)V
 */
JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1createJNIDictionary
  (JNIEnv * env, jobject obj, jbyteArray arr, jint bucketsize){
    
    string name = "Delfina";

    cout << "Hello, " << name << endl;

    //uint lenStr = sizeof(arr);
    uchar *str = new uchar[3];
    str[0]='a';
    str[1]='b';
    str[2]='\0';
    int lenStr=2;

    jboolean *isCopy = JNI_FALSE;
    jbyte* b = env->GetByteArrayElements( arr, isCopy);
    //uchar *str = (uchar*)b;

      IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
      StringDictionary *dict = NULL;
      string filename = string("aaa");
      dict = new StringDictionaryPFC(it, 20);
      ofstream out("xxx.dic");
    dict->save(out);
    cout << "Hello, " << name << endl;
    printf("JNIDictionary created successfully!!!! \n");

    //env->ReleaseByteArrayElements( arr, b, 0);
    
    return;
  }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    locate
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection_locate
  (JNIEnv * env, jobject obj, jstring str, jint strLen){
    return strLen;
  }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    extract
 * Signature: (II)Ljava/lang/String;
 */
// JNIEXPORT jstring JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection_extract
//   (JNIEnv * env, jobject obj, jint id, jint strLen){
//     jstring str = "";
//   }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    _saveJNIDictionary
 * Signature: (Ljava/io/OutputStream;)V
 */
JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1saveJNIDictionary
  (JNIEnv * env, jobject obj, jobject out){

  }
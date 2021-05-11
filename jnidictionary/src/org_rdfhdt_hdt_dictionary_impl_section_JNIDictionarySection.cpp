#include "org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.h"

#include "../libCSD/StringDictionaryPFC.h"
#include "../libCSD/iterators/IteratorDictStringPlain.h"

#include <stdio.h>

StringDictionary *dict;

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
  (JNIEnv * env, jobject obj, jbyteArray arr, jint bucketsize, jstring type){   

    uint lenStr = env->GetArrayLength(arr); 
    unsigned char* buf = new unsigned char[lenStr];
    env->GetByteArrayRegion (arr, 0, lenStr, reinterpret_cast<jbyte*>(buf));

      IteratorDictString *it = new IteratorDictStringPlain(buf, lenStr);
      // StringDictionary *dict = NULL;
      dict = new StringDictionaryPFC(it, bucketsize);

    const char* dictionary = "jnidictionary-";
    const char* dictType = env->GetStringUTFChars(type, 0);
    const char* ext = ".dic";
    char *filename = new char[strlen(dictionary)+strlen(dictType)+strlen(ext)+1];
    strcpy(filename,dictionary);
    strcat(filename,dictType);
    strcat(filename,ext);
    ofstream out(filename);
    dict->save(out);

    cout << dictType << " jnidictionary for generated successfully" << endl ;

    uint n = dict->numElements();
    cout << "number of elements in dictionary : " << n << endl ;
    int i;
    uint lenp;
    for (i=1; i<=n;i++) {
      uchar* str = dict->extract(i,&lenp);
      int pos=dict->locate(str,lenp);
      cout << "locate : " << i << "," << pos << "," << str << endl ;
    }  
    
    return;
  }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    locate
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection_locate
  (JNIEnv * env, jobject obj, jstring str, jint strLen){

    const char* query = env->GetStringUTFChars(str, 0);
    uchar* q = (uchar*)q;
    int pos = dict->locate(q,strLen);

    return pos;
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
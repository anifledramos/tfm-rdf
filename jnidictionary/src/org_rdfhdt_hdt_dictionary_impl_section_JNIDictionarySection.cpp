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

    uint lenStr = sizeof(arr);  
    uint16_t i;  

    printf("size of array:%d\n",lenStr);
    
    jboolean *isCopy = JNI_FALSE;
    jbyte* b = env->GetByteArrayElements( arr, isCopy);
    uchar *str = (uchar*)b;

    for (i=0; i<lenStr; i++)
    {
      cout << "array pos " << i << " : " << str << "\n" ;
    }

      IteratorDictString *it = new IteratorDictStringPlain(str, lenStr);
      StringDictionary *dict = NULL;
      dict = new StringDictionaryPFC(it, bucketsize);
      ofstream out("jnidictionary.dic");

    uchar *query = (uchar*)"http://example.org/predicate6";
    uint len = sizeof(query);    
    uint location = dict->locate(query,len);
    cout << "locate : " << query  << " --> location : "<< location << ";;\n";

    std::size_t id = sizeof(location);
    uint *lenp = &len;
    cout << "id : " << id << ";;;\n";
    cout << "extract : " << dict->extract(id,lenp) << ";;\n"  ;

    dict->save(out);
    printf("JNIDictionary generated successfully \n");
    
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
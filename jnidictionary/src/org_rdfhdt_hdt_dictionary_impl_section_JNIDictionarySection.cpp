#include "org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.h"

#include "../libCSD/StringDictionaryPFC.h"
#include "../libCSD/iterators/IteratorDictStringPlain.h"

#include <stdio.h>

jstring util_char_to_jstring(JNIEnv *env, const char *pat) {
  jclass strClass = (env)->FindClass("java/lang/String");
  jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
  jbyteArray bytes = (env)->NewByteArray(strlen(pat));
  (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
  jstring encoding = (env)->NewStringUTF("utf-8");
  return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}

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
 * Signature: ([BILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1createJNIDictionary
  (JNIEnv * env, jobject obj, jbyteArray arr, jint bucketsize, jstring type){   

    uint lenStr = env->GetArrayLength(arr); 
    unsigned char* buf = new unsigned char[lenStr];
    env->GetByteArrayRegion (arr, 0, lenStr, reinterpret_cast<jbyte*>(buf));

      IteratorDictString *it = new IteratorDictStringPlain(buf, lenStr);
      StringDictionary *dict = new StringDictionaryPFC(it, bucketsize);

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
      cout << "locate : " << i << "," << pos << "," << str << lenp << endl ;
    }  
    
    return (jlong) dict;
  }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    locate
 * Signature: (Ljava/lang/String;IJ)I
 */
JNIEXPORT jint JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection_locate
  (JNIEnv * env, jobject obj, jstring str, jint strLen, jlong jnidictionary){

    const char* query = env->GetStringUTFChars(str, 0);
    uchar* q = (uchar*)query;
    StringDictionary * dict = (StringDictionary *) jnidictionary;
    int pos = dict->locate(q,strLen);    
    
    return pos;
  }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    extract
 * Signature: (IJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection_extract
  (JNIEnv * env, jobject obj, jint id, jlong jnidictionary){

    uint len = (uint)id;
    StringDictionary * dict = (StringDictionary *) jnidictionary;
    uchar* res = dict->extract(id,&len); 

    const char* qid = (const char*)res;
    jstring str = util_char_to_jstring(env,qid);

    return str;
  }

/*
 * Class:     org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection
 * Method:    _saveJNIDictionary
 * Signature: (Ljava/io/OutputStream;)V
 */
JNIEXPORT void JNICALL Java_org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection__1saveJNIDictionary
  (JNIEnv * env, jobject obj, jobject out){

  }


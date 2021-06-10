CPP=g++

OBJECTS=

OBJECTS_JNI=obj/org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.o
BASELIB=./libCSD/dictionary.a

CPPFLAGS_JNI=-std=c++11 -shared -fPIC -O3
INCLUDES_JNI=-I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -I libCSD/libcds/includes

LIBNAME=lib/libjnidictionary.so
 
jni: 
	g++ $(CPPFLAGS_JNI) $(INCLUDES_JNI) src/org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.cpp $(BASELIB) -o $(LIBNAME)
	sudo cp ./lib/libjnidictionary.so /usr/lib/
			

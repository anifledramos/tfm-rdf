CPP=g++

OBJECTS=

OBJECTS_JNI=obj/org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.o

#OBJECTS_BUILD=obj/kTree-construccion.o obj/jnidictionary.o obj/jnidictionary-builder.o obj/commons-indices.o obj/directcodesI.o obj/MemoryManager.o obj/hash.o obj/bitrankw32int.o obj/directcodes.o obj/mymalloc.o obj/basic.o

BINS=build_tree use_tree

#CPPFLAGS=-Wall -g3 
#CPPFLAGS=-O9
CPPFLAGS=-Wall -g -fPIC -shared 
CPPFLAGS_NOWARN=-w -g -shared 

CPPFLAGS_JNI=-shared -fPIC -O3 -Wno-deprecated -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unknown-warning-option
INCLUDES_JNI=-I /usr/local/include -I /usr/local/include/linux -I ./include -I /opt/local/include -I /usr/include -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux -I libCSD/libcds/includes

LIBNAME=lib/libjnidictionary.so
 
DEST=bin

obj/%.o: src/%.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

obj-construccion/%.o: src-construccion/%.c
	$(CPP) $(CPPFLAGS_NOWARN) -c $< -o $@

all: package library bin jni

bin: $(OBJECTS) $(BINS)

package: $(OBJECTS)
	ar rc jnidictionary.a $(OBJECTS)

library: $(OBJECTS)
	$(CPP) -shared -fPIC -O3 -o libktriples.so $(OBJECTS)

build_tree:
	gcc $(CPPFLAGS) -o $(DEST)/build_tree src/pruebaarbol.c jnidictionary.a -lm

use_tree:
	gcc $(CPPFLAGS) -o $(DEST)/use_tree src/usararbol.c $(OBJECTS) -lm

jni_interface:
	g++ $(CPPFLAGS) $(INCLUDES_JNI) -L lib/libcds.a -c src/org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.c -o obj/org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.o

jni: jni_interface
	g++ $(CPPFLAGS_JNI) $(INCLUDES_JNI) -o $(LIBNAME) $(OBJECTS) dictionary.a obj/org_rdfhdt_hdt_dictionary_impl_section_JNIDictionarySection.o -lm
			
clean:
	rm -f $(OBJECTS) i$(OBJECTS_BUILD) $(BINS)
	cd $(DEST); rm -f *.a $(BINS)

CPP=gcc

OBJECTS=obj/mymalloc.o obj/merge.o obj/basic.o obj/bitrankw32int.o obj/directcodesI.o obj/directcodes.o obj/kTreeAux.o obj/kTreeCreation.o obj/kTree.o  obj/interfazSO.o obj/kTreeJoin.o obj/kTreeJoinInt.o obj/hash.o obj/MemoryManager.o obj/joinsFF.o obj/joinsFV.o obj/joinsVV.o obj/simplePatternsF.o obj/simplePatterns.o obj/k2triples.o obj/commons-indices.o obj/k2triples-builder.o

OBJECTS_JNI=obj/org_rdfhdt_hdt_triples_impl_K2Triples.o

#OBJECTS_BUILD=obj/kTree-construccion.o obj/k2triples.o obj/k2triples-builder.o obj/commons-indices.o obj/directcodesI.o obj/MemoryManager.o obj/hash.o obj/bitrankw32int.o obj/directcodes.o obj/mymalloc.o obj/basic.o

BINS=build_tree use_tree

#CPPFLAGS=-Wall -g3 
#CPPFLAGS=-O9
CPPFLAGS=-Wall -g -fPIC -shared 
CPPFLAGS_NOWARN=-w -g -shared 

CPPFLAGS_JNI=-shared -fPIC -O3 -Wno-deprecated -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-unused-but-set-variable -Wno-unknown-warning-option
INCLUDES_JNI=-I /usr/local/include -I /usr/local/include/linux -I ./include -I /opt/local/include -I /usr/include -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux


LIBNAME=lib/libk2triples.so
 
DEST=bin

obj/%.o: src/%.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

obj-construccion/%.o: src-construccion/%.c
	$(CPP) $(CPPFLAGS_NOWARN) -c $< -o $@

all: package library bin jni

bin: $(OBJECTS) $(BINS)

package: $(OBJECTS)
	ar rc k2triples.a $(OBJECTS)

library: $(OBJECTS)
	$(CPP) -shared -fPIC -O3 -o libktriples.so $(OBJECTS)

build_tree:
	gcc $(CPPFLAGS) -o $(DEST)/build_tree src/pruebaarbol.c k2triples.a -lm

use_tree:
	gcc $(CPPFLAGS) -o $(DEST)/use_tree src/usararbol.c $(OBJECTS) -lm

jni_interface:
	gcc $(CPPFLAGS) $(INCLUDES_JNI) -c src/org_rdfhdt_hdt_triples_impl_K2Triples.c -o obj/org_rdfhdt_hdt_triples_impl_K2Triples.o

jni: jni_interface
	gcc $(CPPFLAGS_JNI) $(INCLUDES_JNI) -o $(LIBNAME) $(OBJECTS) obj/org_rdfhdt_hdt_triples_impl_K2Triples.o -lm

#join_tree:
#	gcc $(CPPFLAGS) -o $(DEST)/join_tree src/usararbolJoinsFF.c $(OBJECTS) -lm

#join_treeSS:
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeSS src/usararbolJoinsSS-FF.c $(OBJECTS) -lm

#join_treeOO:
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeOO src/usararbolJoinsOO-FF.c $(OBJECTS) -lm

#join_treeFV: 
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeFV src/usararbolJoinsFV.c $(OBJECTS) -lm

#join_treeFVSS: 
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeFVSS src/usararbolJoinsSS-FV.c $(OBJECTS) -lm

#join_treeFVOO: 
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeFVOO src/usararbolJoinsOO-FV.c $(OBJECTS) -lm

#join_treeVV: 
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeVV src/usararbolJoinsVV.c $(OBJECTS) -lm

#join_treeVVSS: 
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeVVSS src/usararbolJoinsSS-VV.c $(OBJECTS) -lm
#
#join_treeVVOO: 
#	gcc $(CPPFLAGS) -o $(DEST)/join_treeVVOO src/usararbolJoinsOO-VV.c $(OBJECTS) -lm
#
#query:
#	gcc $(CPPFLAGS) -o $(DEST)/query src/queryk2triples.c $(OBJECTS) -lm
#
#test_tree:
#	gcc $(CPPFLAGS) -o $(DEST)/test_tree src/tiemposarbol.c $(OBJECTS) -lm
#
#revtest_tree:
#	gcc $(CPPFLAGS) -o $(DEST)/revtest_tree src/tiemposinverso.c $(OBJECTS) -lm
#	
#rebuild_tree:
#	gcc $(CPPFLAGS) -o $(DEST)/rebuild_tree src/recuperararbol.c $(OBJECTS) -lm
#
#invrebuild_tree:
#	gcc $(CPPFLAGS) -o $(DEST)/invrebuild_tree src/recuperarinverso.c $(OBJECTS) -lm
				
clean:
	rm -f $(OBJECTS) i$(OBJECTS_BUILD) $(BINS)
	cd $(DEST); rm -f *.a $(BINS)


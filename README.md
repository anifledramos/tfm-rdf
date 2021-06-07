# tfm-rdf

### Delfina Ramos - TFM: RDF Compression with HDT and K2Triples

## Link to Overleaf

https://www.overleaf.com/8534796694hpfcpbtbwkpq

## Compilation instructions

### First make sure you compiled hdt-jni
$ cd ../hdt-jni

hdt-jni $ mvn install

### Compile Java to create headers file for native methods
#### JNITriples
javac -cp ../../../target/hdt-java-core-2.0.jar:../../../../hdt-api/target/hdt-api-2.0.jar -h . org/rdfhdt/hdt/triples/impl/JNITriples.java 

#### JNIDictionary
javac -cp ../../../target/hdt-java-core-2.0.jar:../../../../hdt-api/target/hdt-api-2.0.jar -h . org/rdfhdt/hdt/dictionary/impl/section/JNIDictionarySection.java 

### Generate an HDT file using the hdt-java library 
hdt-jni/hdt-java-cli$ ./bin/rdf2hdt.sh ../hdt-java-core/data/test.nt ../hdt-java-core/data/test.hdt

### Now get to the hdt-jena dir and compile it
hdt-jni $ cd ../hdt-jena
hdt-jni $ mvn install

### Compile JNITriples
jnitriples $ make jni

### Compile JNIDictionary
jnidictionary $ make jni

### Execute SPARQL Query against the file.
$ ./bin/hdtsparql.sh ../hdt-java-core/data/test.hdt "SELECT ?s ?p ?o WHERE { ?s ?p ?o . }"

### Execute Triples Query against the file
$ ./bin/hdtSearch.sh ../hdt-java-core/data/test.hdt

## Generate Bechmanrk data with Berlin Benchmark

./generate -fc -pc 70000 -fn databerlin70

## Test sparql
./bin/rdf2hdt.sh ~/TFM/data/bsbmtools-v0.2/bsbmtools-0.2/databerlin80.nt databerlin.hdt

# Run docker
sudo docker build -t rdfjni --rm=true .

sudo docker run --name jnidocker -it rdfjni

sudo docker ps

sudo docker start 0b31db161807 #containerid

sudo docker exec -it jnidocker /bin/bash

sudo docker exec -it gccdocker /bin/bash

ctr+q to quit
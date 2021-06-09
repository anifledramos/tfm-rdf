# tfm-rdf

### Delfina Ramos - TFM: RDF Compression with HDT and K2Triples

## Link to Overleaf

https://www.overleaf.com/8534796694hpfcpbtbwkpq

## Compilation instructions

### First make sure you compiled hdt-jni
$ cd ../hdt-jni

hdt-jni $ mvn -DskipTests install

### Compile Java to create headers file for native methods
#### JNITriples
hdt-jni/hdt-java-core/src/main/java$ javac -cp ../../../target/hdt-java-core-2.0.jar:../../../../hdt-api/target/hdt-api-2.0.jar -h . org/rdfhdt/hdt/triples/impl/JNITriples.java 

#### JNIDictionary
hdt-jni/hdt-java-core/src/main/java$ javac -cp ../../../target/hdt-java-core-2.0.jar:../../../../hdt-api/target/hdt-api-2.0.jar -h . org/rdfhdt/hdt/dictionary/impl/section/JNIDictionarySection.java 

### Generate an HDT file using the hdt-java library 
hdt-jni/hdt-java-cli$ ./bin/rdf2hdt.sh ../../nt/test.nt ../../nt/test.hdt

### Now get to the hdt-jena dir and compile it
hdt-jni $ cd ../hdt-jena

hdt-jni/hdt-jena $ mvn -DskipTests install

### Compile JNITriples
jnitriples $ make clean all

### Compile JNIDictionary
jnidictionary $ make jni

### Execute SPARQL Query against the file.
hdt-jni/hdt-jena$ ./bin/hdtsparql.sh ../../nt/test.hdt "SELECT ?s ?p ?o WHERE { ?s ?p ?o . }"

### Execute Triples Query against the file
hdt-jni/hdt-java-cli$$ ./bin/hdtSearch.sh ../../nt/test.hdt

## Generate Bechmanrk data with Berlin Benchmark

Download data generator from http://wifo5-03.informatik.uni-mannheim.de/bizer/berlinsparqlbenchmark/

Unzip and run generator

./generate -fc -pc 70000 -fn databerlin70

# Run docker

Copy the datasets you want to use to the "data" folder

## Automatic execution (script)

./goDocker.sh

## Manual execution

### Build docker from dockerfile
docker build -t rdfjni --rm=true .

### Create and run container
docker run --name gccdocker --mount type=bind,source="$(pwd)/data",target=/data -it rdfjni

### Execute existing container (must be started)
docker exec -it gccdocker /bin/bash

## Stop existing container
docker stop  #container

## Remove existing container (must be stopped)
docker rm #container

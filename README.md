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

## Build docker from dockerfile
Move N-Triples file to working directory with name "berlin.nt"
docker build -t rdfjni --rm=true .

## Create and run container
docker run --name gccdocker --mount type=bind,source="$(pwd)/data",target=/data -it rdfjni

## List containers
docker container ls -a

## Start existing container
docker start  #containerid

## Execute existing container (must be started)
docker exec -it gccdocker /bin/bash

## Stop existing container
docker stop  #containerid

## Remove existing container (must be stopped)
docker container rm #containerid

## Exit docker
exit to quit in gcc
ctr+q to quit in openjdk

# Testing against Xermade server
ssh -p 2218 delfina@xermade.dc.fi.udc.es

## Copy files to xermade
scp -P 2218 files delfina@xermade.dc.fi.udc.es:~
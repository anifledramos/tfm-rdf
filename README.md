# (MUCD) TFM: SPARQL evaluation over compact data structures

* Author: María Delfina Ramos Vidal  
* Tutor: Óscar Corcho García
* Tutor: Guillermo de Bernardo Roca 

## Compilation instructions

## Automatic preparation

    ./prepare.sh

## Manual preparation

### First make sure you compiled hdt-jni

    hdt-jni $ mvn -DskipTests install

### Now get to the hdt-jena dir and compile it

    hdt-jni/hdt-jena $ mvn -DskipTests install

### Compile Java to create headers file for native methods (only if you are implementing new JNI Adapters, not necessary to compile and run the program)

* JNITriples

      hdt-jni/hdt-java-core/src/main/java$ javac -cp ../../../target/hdt-java-core-2.0.jar:../../../../hdt-api/target/hdt-api-2.0.jar -h . org/rdfhdt/hdt/triples/impl/JNITriples.java 

* JNIDictionary

      hdt-jni/hdt-java-core/src/main/java$ javac -cp ../../../target/hdt-java-core-2.0.jar:../../../../hdt-api/target/hdt-api-2.0.jar -h . org/rdfhdt/hdt/dictionary/impl/section/JNIDictionarySection.java 

### Compile JNITriples and save library libjnitriples
    jnitriples $ make clean all

Makefile will try to copy the generated library into /usr/lib directory and succeed in Docker but fail in local. Prepare.sh copies the libjnitriples.so file generated to /usr/lib or you can manually move the library to /usr/lib if you are going step by step.

    sudo cp ./lib/libjnitriples.so /usr/lib/

### Compile JNIDictionary and save library libjnidictionary
    jnidictionary $ make jni

Makefile will try to copy the generated library into /usr/lib directory and succeed in Docker but fail in local. Prepare.sh copies the libjnidictionary.so file generated to /usr/lib or you can manually move the library to /usr/lib if you are going step by step.

    sudo cp ./lib/libjnidictionary.so /usr/lib/

### Generate an HDT file using the hdt-java library 

Usage: `rdf2hdt [options] <Input RDF> <Output HDT>`

    Options:
      -base      Base URI for the dataset
      -config    Conversion config file
      -options   HDT Conversion options
      -rdftype   Type of RDF Input (ntriples, n3, rdfxml)

#### Generate plain HDT dataset

`./bin/rdf2hdt.sh -options "dictionary.type=<http://purl.org/HDT/hdt#dictionaryFour>;triples.format=<http://purl.org/HDT/hdt#triplesBitmap>;" ../../nt/test.nt ../../nt/test.hdt`

#### Generate K2-tree + PFC dataset

`./bin/rdf2hdt.sh -options "dictionary.type=<http://purl.org/HDT/hdt#dictionaryJNI>;triples.format=<http://purl.org/HDT/hdt#triplesJNI>;" ../../nt/test.nt ../../nt/test.hdt`

### Execute SPARQL Query against the file.
    hdt-jni/hdt-jena$ ./bin/hdtsparql.sh path/to/test.hdt "SELECT ?s ?p ?o WHERE { ?s ?p ?o . }"

### Execute Triples Query against the file
    hdt-jni/hdt-java-cli$ ./bin/hdtSearch.sh path/to/test.hdt

## Generate Bechmanrk data with Berlin Benchmark

Download data generator from http://wifo5-03.informatik.uni-mannheim.de/bizer/berlinsparqlbenchmark/

Unzip and run generator

    ./generate -fc -pc 100000 -fn datasetname

# Run docker

Copy the datasets you want to use to the "data" folder

## Automatic execution (script)

    ./goDocker.sh

## Manual execution

### Build docker from dockerfile
    docker build -t rdfjni --rm=true .

### Create and run container
    docker run --name hdtdocker --mount type=bind,source="$(pwd)/data",target=/data -it rdfjni

### Execute existing container (must be started)
    docker exec -it hdtdocker /bin/bash

### Stop existing container
    docker stop  hdtdocker

### Remove existing container (must be stopped)
    docker rm hdtdocker


# Troubleshooting

The current implementation is designed for Linux based system and can have issues running on Windows or Mac. We recommend running the prototype in Docker. 
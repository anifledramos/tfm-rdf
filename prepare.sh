git clone https://github.com/anifledramos/tfm-rdf.git

cd tfm-rdf/

cd jnitriples/

mkdir obj 

make clean all

cd ../jnidictionary 

make jni

cd ../hdt-jni 

mvn -DskipTests install

cd hdt-java-cli/ 

./bin/rdf2hdt.sh ../../nt/test.nt ../../nt/test.nt

mvn clean install
#git clone https://github.com/anifledramos/tfm-rdf.git

#cd tfm-rdf/

cd jnitriples/

mkdir obj 

make clean all

sudo cp ./lib/libjnitriples.so /usr/lib/

cd ../jnidictionary 

make jni

sudo cp ./lib/libjnidictionary.so /usr/lib/

cd ../hdt-jni 

mvn -DskipTests install

cd hdt-java-cli/ 

./bin/rdf2hdt.sh ../../nt/test.nt ../../nt/test.nt

mvn clean install

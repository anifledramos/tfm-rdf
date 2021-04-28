######### How to compile the dictionary library #############

# Within libcds folder
make

# Within libCSD
make clean all
make package

# Copy generated dictionary.a to usr/lib
sudo cp dictionary.a /usr/lib/

# within foot folder
make jni

# copy generated libjnidictionary.so to /usr/lib/
sudo cp libjnidictionary.so /usr/lib/
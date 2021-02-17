CPP=gcc

OBJECTS=basic.o bitrankw32int.o directcodes.o kTree.o hash.o MemoryManager.o
BINS=build_tree rebuild_tree
#CPPFLAGS=-Wall -g3 
#CPPFLAGS=-O9
CPPFLAGS=-Wall -O9 -g -DNDEBUG -w
DEST=.

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: bin

bin: $(OBJECTS) $(BINS)


build_tree:
	gcc $(CPPFLAGS) -o $(DEST)/build_tree pruebaarbol.c $(OBJECTS) -lm

rebuild_tree:
	gcc $(CPPFLAGS) -o $(DEST)/rebuild_tree recuperararbol.c $(OBJECTS) -lm
				
clean:
	rm -f $(OBJECTS) $(BINS)
	cd $(DEST); rm -f *.a $(BINS)


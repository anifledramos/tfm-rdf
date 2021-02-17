CPP=gcc

OBJECTS=basic.o bitrankw32int.o directcodes.o
BINS=create_dac create_indexOP comprobador
#CPPFLAGS=-Wall -g3 
#CPPFLAGS=-O9
CPPFLAGS=-Wall -O9 -g -DNDEBUG -w 
DEST=.

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: bin

bin: $(OBJECTS) $(BINS)

create_dac:
	gcc $(CPPFLAGS) -o $(DEST)/create_dac main.c $(OBJECTS) -lm

create_indexOP: 
	gcc $(CPPFLAGS) -o $(DEST)/create_indexOP indexOP.c -lm			
comprobador:
	gcc $(CPPFLAGS) -o comprobador comprobador.c	
clean:
	rm -f $(OBJECTS) $(BINS)
	rm -f 
	cd $(DEST); rm -f *.a $(BINS)


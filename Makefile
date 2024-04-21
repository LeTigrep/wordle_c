CC = gcc
CFLAGS = -Wall -Wfatal-errors -g
LDFLAGS = -pthread
OBJECTS = mots_5_lettres.o wordle_simple.o serveur.o 
OBJECTS2 = client.o
PROGS = wordle
PROGS2 = client

all: $(PROGS) $(PROGS2) 

wordle: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)
	
client: $(OBJECTS2)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS2)


mots_5_lettres.o: mots_5_lettres.c mots_5_lettres.h
wordle_simple.o: wordle_simple.c mots_5_lettres.h
serveur.o: serveur.c
client.o: client.c


.PHONY: clean

clean:
	rm -f $(OBJECTS) $(PROGS) $(OBJECTS2)

CC=gcc
CFLAGS=-I.

all :  s c lib.o

s : lib.o 
	$(CC) -lpthread lib.c server.c -o s $(CFLAGS)

c : lib.o
	$(CC) client.c lib.c -o c $(CFLAGS)

lib.o : lib.h
	$(CC) -c lib.c -o lib.o $(CFLAGS)

clean:
	rm s 
	rm c
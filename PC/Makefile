CC=gcc
CFLAGS=-Wall 
all: a.out clean


a.out: mainPC.o funciones.o
	$(CC) $(CFLAGS) mainPC.o funciones.o -o a.out

mainPC.o: mainPC.c funciones.h
	$(CC) $(CFLAGS) -c mainPC.c -o mainPC.o

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c -o funciones.o

# Elimina todos los archivos objetos (extesión .o)
clean:
	rm  -f *.o

CC=gcc
CFLAGS=-Wall 
CLIB=-lwiringPi
all: a.out clean


a.out: mainRaspi.o easyPio.o funciones.o funcionesAsmARM.o
	$(CC) $(CFLAGS) mainRaspi.o easyPio.o funciones.o funcionesAsmARM.o -o a.out $(CLIB)

mainRaspi.o: mainRaspi.c easyPio.h funciones.h
	$(CC) $(CFLAGS) -c mainRaspi.c -o mainRaspi.o

easyPio.o: easyPio.c easyPio.h
	$(CC) $(CFLAGS) -c easyPio.c -o easyPio.o

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c -o funciones.o $(CLIB)

funcionesAsmARM.o: funcionesAsmARM.s
	$(CC) $(CFLAGS) -c funcionesAsmARM.s -o funcionesAsmARM.o

# Elimina todos los archivos objetos (extesión .o)
clean:
	rm  mainRaspi.o easyPio.o funciones.o funcionesAsmARM.o

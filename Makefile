# PROJET : Fichier Makefile
#
include ../Makefile.inc

EXE = serveur client

${EXE): ${PSE_LIB }

all: ${EXE pkg-config --cflags --libs gtk+-3.0 gmodule-2.0}

clean:
	rm -f *.o *~ ${EXE}



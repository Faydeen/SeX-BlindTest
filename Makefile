# PROJET : Fichier Makefile
#
include ../Makefile.inc

EXE = serveur 

${EXE): ${PSE_LIB}

all: ${EXE}

clean:
	rm -f *.o *~ ${EXE} 



COMPILATEUR = gcc
CFLAGS = -Wall
EXEC = tsp
SOURCES = main.c tools.c force_brute.c plusprochevoisin.c
LIBS = -lm
FILES_o = main.o tools.o force_brute.o plusprochevoisin.o
FICHIER_TEST = Data/att10.tsp
METHODE = -bf

all:
	$(COMPILATEUR) $(CFLAGS) -c $(SOURCES)
	$(COMPILATEUR) -o $(EXEC) $(FILES_o) $(LIBS)
	make clean

run : all
	./$(EXEC) -f $(FICHIER_TEST) $(METHODE)
	make clean

clean :
	rm $(FILES_o)


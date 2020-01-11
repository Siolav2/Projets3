COMPILATEUR = gcc
CFLAGS = -Wall
EXEC = tsp
SOURCES = main.c tools.c force_brute.c plusprochevoisin.c randomwalk.c opt2.c algo_genet.c
LIBS = -lm
FILES_o = main.o tools.o force_brute.o plusprochevoisin.o randomwalk.o opt2.o algo_genet.o
FICHIER_TEST = Data/att10.tsp
METHODE = -ga

all:
	$(COMPILATEUR) $(CFLAGS) -c $(SOURCES)
	$(COMPILATEUR) -o $(EXEC) $(FILES_o) $(LIBS)
	make clean

run : all
	./$(EXEC) -f $(FICHIER_TEST) $(METHODE)
	make clean

clean :
	rm $(FILES_o)


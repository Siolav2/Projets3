COMPILATEUR = gcc
CFLAGS = -Wall -Wextra
EXEC = tsp
SOURCES = main.c tools.c force_brute.c
LIBS = -lm
FILES_o = main.o tools.o force_brute.o

all:
	$(COMPILATEUR) $(CFLAGS) -c $(SOURCES)
	$(COMPILATEUR) -o $(EXEC) $(FILES_o) $(LIBS)
	make clean

run : all
	./$(EXEC)
	make clean

clean :
	rm $(FILES_o)

